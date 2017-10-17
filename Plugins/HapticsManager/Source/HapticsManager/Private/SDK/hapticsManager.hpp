#ifndef BHAPTICS_HPP
#define BHAPTICS_HPP

#include "thirdparty/easywsclient.hpp"
#include "common/timer.hpp"
#include "common/model.hpp"
#include "common/util.hpp"
#include "Engine.h"

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>

namespace bhaptics
{
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))  
    using namespace std;
    using easywsclient::WebSocket;
    class HapticPlayer
    {
		static HapticPlayer *hapticManager;
        unique_ptr<WebSocket> ws;

        map<string, BufferedHapticFeedback> _registeredSignals;
        map<string, BufferedHapticFeedback> _activeSignals;
        mutex mtx;
        int _currentTime = 0;
        int _interval = 20;
        int _motorSize = 20;
        HapticTimer timer;

		bool _enable = false;

        string host = "127.0.0.1";
        int port = 15881;
        string path = "feedbacks";

        int reconnectSec = 5;
        std::chrono::steady_clock::time_point prevReconnect;

		HapticPlayer() {}

        void reconnect()
        {
			if (!_enable)
			{
				return;
			}

            if (!retryConnection)
            {
                return;
            }

            if (connectionCheck())
            {
                return;
            }

            std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();

            int values = std::chrono::duration_cast<std::chrono::seconds>(current - prevReconnect).count();

            if (values > reconnectSec)
            {
                auto tried = unique_ptr<WebSocket>(WebSocket::create(host, port, path));
                if (tried)
                {
					tried->close();
					tried->poll();
                    ws.reset(WebSocket::create(host, port, path));
                }
	            ws.reset(WebSocket::create(host, port, path));
                prevReconnect = current;
            }
        }

        void playFeedback(const HapticFeedback &feedback)
        {
            vector<uint8_t> message(_motorSize + 2, 0);
            if (feedback.mode == PATH_MODE)
            {
                message[0] = 1;
            }
            else if (feedback.mode == DOT_MODE)
            {
                message[0] = 2;
            }

            message[1] = (uint8_t)feedback.position;

            for (int i = 0; i < _motorSize; i++)
            {
                message[i + 2] = feedback.values[i];
            }

            send(message);
        }

		void playFeedbackFrame(const HapticFeedbackFrame &feedback)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

			if (!_enable)
			{
				vector<DotPoint> emptyPoints;
				HapticFeedbackFrame turnOffFrame(feedback.position,emptyPoints);
				bhaptics::to_json(*JsonObject, turnOffFrame);
			}
			else
			{
				bhaptics::to_json(*JsonObject, feedback);
			}

			FString OutputString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			std::string jStr(TCHAR_TO_UTF8(*OutputString));
			sendStr(jStr);
		}

        bool connectionCheck()
        {
            if (!ws)
            {
                return false;
            }

            if (ws->getReadyState() == WebSocket::CLOSED)
            {
                ws.reset();
                return false;
            }

            return true;
        }

        void send(const vector<uint8_t> &message)
        {
            if (!connectionCheck())
            {
                return;
            }
            ws->sendBinary(message);
            ws->poll();
        }

		void sendStr(const std::string &message)
		{
			if (!connectionCheck())
			{
				return;
			}

			ws->send(message);
			ws->poll();

		}

        void updateActive(const string &key, const BufferedHapticFeedback& signal)
        {
            mtx.lock();
            _activeSignals[key] = signal;
            mtx.unlock();
        }

        void remove(const string &key)
        {
            mtx.lock();
            _activeSignals.erase(key);
            mtx.unlock();
        }

        void removeAll()
        {
            mtx.lock();
            _activeSignals.clear();
            mtx.unlock();
        }

        void doRepeat()
        {
            reconnect();

			checkMessage();

            if (_activeSignals.size() == 0)
            {
                if (_currentTime > 0)
                {
                    _currentTime = 0;
					vector<DotPoint> points;
					HapticFeedbackFrame rightFeedback(Right, points);
					HapticFeedbackFrame leftFeedback(Left, points);
					HapticFeedbackFrame vestFrontfeedback(VestFront, points);
					HapticFeedbackFrame vestBackFeedback(VestBack, points);
					HapticFeedbackFrame headFeedback(Head, points);
					playFeedbackFrame(rightFeedback);
					playFeedbackFrame(leftFeedback);
					playFeedbackFrame(vestFrontfeedback);
					playFeedbackFrame(vestBackFeedback);
					playFeedbackFrame(headFeedback);
                }

                return;
            }

            vector<string> expiredSignals;

			map<Position, HapticFeedbackFrame> map;
			vector<DotPoint> emptyPoints;
			HapticFeedbackFrame left(Left, emptyPoints);
			HapticFeedbackFrame right(Right, emptyPoints);
			HapticFeedbackFrame vFront(VestFront, emptyPoints);
			HapticFeedbackFrame vBack(VestBack, emptyPoints);
			HapticFeedbackFrame head(Head, emptyPoints);
            map[Left] = left;
            map[Right] = right;
            map[VestFront] = vFront;
            map[VestBack] = vBack;
            map[Head] = head;

            mtx.lock();
            for (auto keyPair = _activeSignals.begin(); keyPair != _activeSignals.end(); ++keyPair)
            {
                auto key = keyPair->first;

                if (keyPair->second.StartTime > _currentTime || keyPair->second.StartTime < 0)
                {
                    keyPair->second.StartTime = _currentTime;
                }

                int timePast = _currentTime - keyPair->second.StartTime;

                if (timePast > keyPair->second.EndTime)
                {
                    expiredSignals.push_back(key);
                }
                else
                {
                    if (Common::containsKey(timePast, keyPair->second.feedbackMap))
                    {
                        auto hapticFeedbackData = keyPair->second.feedbackMap.at(timePast);
                        for (auto &feedback : hapticFeedbackData)
                        {
                            //map[feedback.position] = feedback;
							map[feedback.position].dotPoints.insert(map[feedback.position].dotPoints.end(), feedback.dotPoints.begin(), feedback.dotPoints.end());
							map[feedback.position].pathPoints.insert(map[feedback.position].pathPoints.end(), feedback.pathPoints.begin(), feedback.pathPoints.end());
							map[feedback.position].texture = feedback.texture;
                        }
                    }
                }
            }
			
			for (auto keyValue = map.begin(); keyValue != map.end(); ++keyValue)
			{
				auto frame = keyValue->second;
				playFeedbackFrame(frame);
			}

            mtx.unlock();

            for (auto &key : expiredSignals)
            {
                remove(key);
            }

            _currentTime += _interval;
        }

        void callbackFunc()
        {
            doRepeat();
        }


    public:
        bool retryConnection = true;

        int registerFeedback(const string &key, const string &filePath)
        {
            try
            {
                HapticFile file = Util::parse(filePath);
                BufferedHapticFeedback signal(file);
                _registeredSignals[key] = signal;

                return 0;
            } catch(exception &e)
            {
                printf("Exception : %s\n", e.what());

                return -1;
            }
        }

        void init()
        {
			if (_enable)
				return;
            function<void()> callback = std::bind(&HapticPlayer::callbackFunc, this);
            timer.addTimerHandler(callback);
            timer.start();

#ifdef _WIN32
            INT rc;
            WSADATA wsaData;

            rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (rc) {
                printf("WSAStartup Failed.\n");
                return;
            }
#endif

            try
            {
                ws = unique_ptr<WebSocket>(WebSocket::create(host, port, path));

                connectionCheck();
            }
            catch (exception &e)
            {
                printf("Exception : %s\n", e.what());
            }

            /*vector<uint8_t> values(_motorSize, 0);
            HapticFeedback feedback(Right, values, DOT_MODE);
            playFeedback(feedback);*/
			_enable = true;
			vector<DotPoint> points;
			HapticFeedbackFrame feedback(Right,points);
			playFeedbackFrame(feedback);
        }

        void submit(const string &key, Position position, const vector<uint8_t> &motorBytes, int durationMillis)
        {
			vector<DotPoint> points;
			for (int i = 0; i < motorBytes.size(); i++)
			{
				if (motorBytes[i] > 0)
				{
					points.push_back(DotPoint(i,motorBytes[i]));
				}
			}

            BufferedHapticFeedback signal(position,points, durationMillis, _interval);
            updateActive(key, signal);
        }

		void submit(const string &key, Position position, const vector<DotPoint> &points, int durationMillis)
		{
			BufferedHapticFeedback signal(position, points, durationMillis, _interval);
			updateActive(key, signal);
		}

        void submit(const string &key, Position position, const vector<PathPoint> &points, int durationMillis)
        {

            BufferedHapticFeedback signal(position, points, durationMillis, _interval);
            updateActive(key, signal);
        }

        void submitRegistered(const string &key, float intensity, float duration)
        {
            if (!Common::containsKey(key, _registeredSignals))
            {
                printf("Key : %s is not registered.\n", key.c_str());

                return;
            }

            if (duration < 0.01f || duration > 100.0f)
            {
                printf("not allowed duration %f\n", duration);
                return;
            }

            if (intensity < 0.01f || intensity > 100.0f)
            {
                printf("not allowed intensity %f\n", duration);
                return;
            }

            BufferedHapticFeedback signal = _registeredSignals.at(key);

            BufferedHapticFeedback copiedFeedbackSignal = BufferedHapticFeedback::Copy(signal, _interval, intensity, duration);
            updateActive(key, copiedFeedbackSignal);
        }

        void submitRegistered(const string &key)
        {
            if (!Common::containsKey(key, _registeredSignals))
            {
                printf("Key : '%s' is not registered.\n", key.c_str());

                return;
            }

            auto signal = _registeredSignals.at(key);

            signal.StartTime = -1;
            if (!Common::containsKey(key, _activeSignals))
            {
                updateActive(key, signal);
            } else
            {
                _activeSignals.at(key).StartTime = -1;
            }
            
        }

        bool isPlaying()
        {
            return _activeSignals.size() > 0;
        }

        bool isPlaying(const string &key)
        {
            return Common::containsKey(key, _activeSignals);
        }

        void turnOff()
        {
            removeAll();
        }

        void turnOff(const string &key)
        {
            if (!Common::containsKey(key, _activeSignals))
            {
                printf("feedback with key( %s ) is not playing.\n", key.c_str());
                return;
            }

            remove(key);
        }

		void(*dispatchFunctionVar)(const char*);

		void checkMessage()
		{
			if (!ws)
			{
				return;
			}

			if (dispatchFunctionVar != NULL)
			{
				ws->dispatchChar(dispatchFunctionVar);
			}
			ws->poll();
		}

        void destroy()
        {
            //vector<uint8_t> values(_motorSize, 0);
            //HapticFeedback feedback(All, values, DOT_MODE);
            //playFeedback(feedback);

			vector<DotPoint> points;
			HapticFeedbackFrame feedback(All,points);
			playFeedbackFrame(feedback);
			_enable = false;
			
            if (!ws)
            {
                return;
            }

			ws->close();
			ws->poll();
			ws.reset();
			//delete ws;
        }

		void enableFeedback()
		{
			if (!ws)
			{
				return;
			}
			_enable = true;
		}

		void disableFeedback()
		{
			_enable = false;
		}

		void toggleFeedback()
		{
			if (!ws)
			{
				return;
			}

			_enable = !_enable;
		}

		HapticPlayer(HapticPlayer const&) = delete;
		void operator= (HapticPlayer const&) = delete;

		static HapticPlayer *instance()
		{
			if (!hapticManager)
			{
				hapticManager = new HapticPlayer();
			}

			return hapticManager;
		}
    };
}

#endif