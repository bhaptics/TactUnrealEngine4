//Copyright bHaptics Inc. 2017
#pragma once
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
	private:
		//static HapticPlayer *hapticManager;
		unique_ptr<WebSocket> ws;
		vector<RegisterRequest> _registered;

		vector<string> _activeKeys;
		vector<Position> _activeDevices;

		mutex mtx;// mutex for _activeKeys and _activeDevices variable
		mutex registerMtx; //mutex for _registered variable
		mutex pollingMtx; //mutex for _registered variable

		int _currentTime = 0;
		int _interval = 20;
		int _motorSize = 20;
		HapticTimer timer;

		bool isRunning = false;
		bool isPolling = false;

		bool _enable = false;

		string host = "127.0.0.1";
		int port = 15881;
		string path = "v2/feedbacks";

		int reconnectSec = 5;
		std::chrono::steady_clock::time_point prevReconnect;


		bool isRegisterSent = true;

		void reconnect()
		{

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

				ws.reset(WebSocket::create(host, port, path));

				isRegisterSent = false;

				prevReconnect = current;
			}
		}

		void resendRegistered()
		{
			if (connectionCheck() && _registered.size()>0)
			{
				PlayerRequest req;
				registerMtx.lock();
				vector<RegisterRequest> tempRegister = _registered;
				registerMtx.unlock();


				for (size_t i = 0; i < tempRegister.size(); i++)
				{
					req.Register.push_back(tempRegister[i]);
				}
				send(req);

				isRegisterSent = true;
			}
		}

		bool connectionCheck()
		{
			if (!ws)
			{
				return false;
			}
			pollingMtx.lock();
			WebSocket::readyStateValues isClosed = ws->getReadyState();
			pollingMtx.unlock();
			if (isClosed == WebSocket::CLOSED)
			{
				ws.reset(nullptr);
				return false;
			}

			return true;
		}

		void send(PlayerRequest request)
		{
			if (!connectionCheck())
			{
				return;
			}
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			request.to_json(*JsonObject);
			FString OutputString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			std::string jStr(TCHAR_TO_UTF8(*OutputString));


			/*if (isPolling)
			{
			return;
			}
			isPolling = true;*/
			pollingMtx.lock();
			ws->send(jStr);
			ws->poll();
			pollingMtx.unlock();
			//isPolling = false;
		}

		void updateActive(const string &key, const Frame& signal)
		{
			if (!_enable || !connectionCheck())
			{
				return;
			}

			SubmitRequest req;
			PlayerRequest playerReq;
			req.Frame = signal;
			req.Key = key;
			req.Type = "frame";

			playerReq.Submit.push_back(req);

			send(playerReq);
		}

		void remove(const string &key)
		{
			if (!_enable || !connectionCheck())
			{
				return;
			}

			SubmitRequest req;
			PlayerRequest playerReq;
			req.Key = key;
			req.Type = "turnOff";

			playerReq.Submit.push_back(req);

			send(playerReq);
		}

		void removeAll()
		{
			if (!_enable)
			{
				return;
			}

			SubmitRequest req;
			PlayerRequest playerReq;
			req.Type = "turnOffAll";

			playerReq.Submit.push_back(req);

			send(playerReq);
		}



		void callbackFunc()
		{
			//doRepeat();
		}

	public:
		bool retryConnection = true;

		void doRepeat()
		{
			if (isRunning)
			{
				return;
			}
			isRunning = true;
			reconnect();

			//registerMtx.lock();
			if (!isRegisterSent)
			{
				resendRegistered();
			}
			//registerMtx.unlock();

			checkMessage();

			_currentTime += _interval;
			isRunning = false;
		}

		HapticPlayer() {}

		int registerFeedback(const string &key, const string &filePath)
		{
			HapticFile file = Util::parse(filePath);

			RegisterRequest req;
			req.Key = key;
			req.Project = file.project;

			registerMtx.lock();
			_registered.push_back(req);

			PlayerRequest playerReq;

			playerReq.Register.push_back(req);

			send(playerReq);
			registerMtx.unlock();
			return 0;
		}

		void init()
		{
			if (_enable)
				return;
			//function<void()> callback = std::bind(&HapticPlayer::callbackFunc, this);
			//timer.addTimerHandler(callback);
			//timer.start();

#ifdef _WIN32
			INT rc;
			WSADATA wsaData;

			rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (rc) {
				printf("WSAStartup Failed.\n");
				return;
			}
#endif
			ws = unique_ptr<WebSocket>(WebSocket::create(host, port, path));

			connectionCheck();

			_enable = true;
		}

		void submit(const string &key, Position position, const vector<uint8_t> &motorBytes, int durationMillis)//change to submit - done
		{
			if (!_enable || !connectionCheck())
			{
				return;
			}

			vector<DotPoint> points;
			for (size_t i = 0; i < motorBytes.size(); i++)
			{
				if (motorBytes[i] > 0)
				{
					points.push_back(DotPoint(i, motorBytes[i]));
				}
			}

			Frame submitFrame = Frame::AsDotPointFrame(points, position, durationMillis);
			updateActive(key, submitFrame);
		}

		void submit(const string &key, Position position, const vector<DotPoint> &points, int durationMillis)
		{
			Frame req = Frame::AsDotPointFrame(points, position, durationMillis);
			updateActive(key, req);
		}

		void submit(const string &key, Position position, const vector<PathPoint> &points, int durationMillis)
		{
			Frame req = Frame::AsPathPointFrame(points, position, durationMillis);
			updateActive(key, req);
		}

		void submitRegistered(const string &key, float intensity, float duration)//change to submitRegistered
		{
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

			SubmitRequest req;
			PlayerRequest playerReq;
			req.Key = key;
			req.Type = "key";
			req.Parameters["intensityRatio"] = std::to_string(intensity);
			req.Parameters["durationRatio"] = std::to_string(duration);

			playerReq.Submit.push_back(req);

			send(playerReq);
		}

		void submitRegistered(const string &key)
		{
			if (!_enable || !connectionCheck())
			{
				return;
			}

			SubmitRequest req;
			PlayerRequest playerReq;
			req.Key = key;
			req.Type = "key";

			playerReq.Submit.push_back(req);

			send(playerReq);
		}

		void submitRegistered(const string &key, double deltaX, double deltaY, bool isValueRotate = true)
		{
			if (!_enable || !connectionCheck())
			{
				return;
			}

			TransformOption option;// = TransformOption(deltaX, deltaY, isRotateValue);
			option.DeltaX = deltaX;
			option.DeltaY = deltaY;
			option.IsValueRotate = isValueRotate;
			SubmitRequest req;
			PlayerRequest playerReq;
			req.Key = key;
			req.Type = "key";
			req.Parameters["transformOption"] = option.to_string();

			playerReq.Submit.push_back(req);

			send(playerReq);
		}

		bool isPlaying()
		{
			return _activeKeys.size() > 0;
		}

		bool isPlaying(const string &key)
		{
			mtx.lock();
			vector<string> temp = _activeKeys;
			mtx.unlock();

			bool ret = std::find(temp.begin(), temp.end(), key) != temp.end();
			return ret;
		}

		void turnOff()
		{
			removeAll();
		}

		void turnOff(const string &key)
		{
			remove(key);
		}

		void(*dispatchFunctionVar)(const char*);

		void checkMessage()
		{
			if (!ws)
			{
				return;
			}

			if (dispatchFunctionVar != NULL && _enable)
			{

				/*if (isPolling)
				{
				return;
				}*/
				//isPolling = true;
				if (pollingMtx.try_lock())
				{
					ws->dispatchChar(dispatchFunctionVar);
					ws->poll();
					pollingMtx.unlock();
				}
			}

		}

		void destroy()
		{
			if (!ws)
			{
				return;
			}
			//timer.stop();
			_enable = false; //ensures no more sends when destroying
			dispatchFunctionVar = NULL; //ensures no more dispatches when destroying
										//isPolling = true;
			pollingMtx.lock();
			ws->close();
			ws->poll();
			ws.reset();
			pollingMtx.unlock();
			//isPolling = false;

			_activeDevices.erase(_activeDevices.begin(), _activeDevices.end());
			_activeKeys.erase(_activeKeys.begin(), _activeKeys.end());

			//_enable = false; // allows for sending again, just in case

		}

		void enableFeedback()
		{
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

		map<string, vector<int>> parseResponse(FJsonObject j)
		{
			PlayerResponse response;
			response.from_json(j);
			mtx.lock();
			_activeKeys = response.ActiveKeys;
			_activeDevices = response.ConnectedPositions;
			mtx.unlock();
			return response.Status;
		}

		void parseResponse(PlayerResponse response)
		{
			mtx.lock();
			_activeKeys = response.ActiveKeys;
			_activeDevices = response.ConnectedPositions;
			mtx.unlock();
		}

		bool isDevicePlaying(Position device)
		{
			mtx.lock();
			vector<bhaptics::Position> temp = _activeDevices;
			mtx.unlock();
			bool ret = std::find(temp.begin(), temp.end(), device) != temp.end();
			return ret;
		}

		//HapticPlayer(HapticPlayer const&) = delete;
		//void operator= (HapticPlayer const&) = delete;

		//static HapticPlayer *instance()
		//{
		//	if (!hapticManager)
		//	{
		//		hapticManager = new HapticPlayer();
		//	}

		//	return hapticManager;
		//}
	};
}

#endif