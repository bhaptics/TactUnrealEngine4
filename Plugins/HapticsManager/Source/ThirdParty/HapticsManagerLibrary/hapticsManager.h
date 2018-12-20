//Copyright bHaptics Inc. 2018
#pragma once


#include "easywsclient.h"
#include "json.hpp"
#include "timer.h"
#include "model.h"
//#include "common/util.hpp"

#include <string>
#include <vector>
#include <mutex>
#include <map>

namespace bhaptics
{
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y)) 

	struct PlayerResponse
	{
		std::vector<std::string> RegisteredKeys;
		std::vector<std::string> ActiveKeys;
		int ConnectedDeviceCount;
		std::vector<Position> ConnectedPositions;
		std::map<std::string, std::vector<int>> Status;

		static void from_json(const nlohmann::json& j, PlayerResponse& p)
		{
			p.RegisteredKeys = j.at("RegisteredKeys").get<std::vector<std::string>>();
			p.ActiveKeys = j.at("ActiveKeys").get<std::vector<std::string>>();
			p.ConnectedDeviceCount = j.at("ConnectedDeviceCount").get<int>();

			std::vector<std::string> positionStr = j.at("ConnectedPositions").get<std::vector<std::string>>();

			for (size_t i = 0; i < positionStr.size(); i++)
			{
				std::string DeviceValue = positionStr[i];

				if (DeviceValue == "Left")
				{
					p.ConnectedPositions.push_back(Position::Left);
				}
				else if (DeviceValue == "Right")
				{
					p.ConnectedPositions.push_back(Position::Right);
				}
				if (DeviceValue == "ForearmL")
				{
					p.ConnectedPositions.push_back(Position::Left);
				}
				else if (DeviceValue == "ForearmR")
				{
					p.ConnectedPositions.push_back(Position::Right);
				}
				else if (DeviceValue == "Vest")
				{
					p.ConnectedPositions.push_back(Position::Vest);
				}
				else if (DeviceValue == "Head")
				{
					p.ConnectedPositions.push_back(Position::Head);
				}
				else if (DeviceValue == "Racket")
				{
					p.ConnectedPositions.push_back(Position::Racket);
				}
				else if (DeviceValue == "HandL")
				{
					p.ConnectedPositions.push_back(Position::HandL);
				}
				else if (DeviceValue == "HandR")
				{
					p.ConnectedPositions.push_back(Position::HandR);
				}
				else if (DeviceValue == "FootL")
				{
					p.ConnectedPositions.push_back(Position::FootL);
				}
				else if (DeviceValue == "FootR")
				{
					p.ConnectedPositions.push_back(Position::FootR);
				}
			}

			const nlohmann::json& rh = j["Status"];

			for (auto& element : nlohmann::json::iterator_wrapper(rh)) {
				std::string key = element.key();
				std::vector<int> motors = element.value().get<std::vector<int>>();
				p.Status[key] = motors;
			}
		}
	};

	class HapticPlayer
	{
	private:
		static HapticPlayer *hapticManager;

		std::unique_ptr<easywsclient::WebSocket> ws;
		std::vector<RegisterRequest> _registered;

		std::vector<std::string> _activeKeys;
		std::vector<Position> _activeDevices;

		std::vector<std::string> componentIds;

		std::map<std::string, std::vector<int>> _activeFeedback;

		std::mutex mtx;// mutex for _activeKeys and _activeDevices variable
		std::mutex registerMtx; //mutex for _registered variable
		std::mutex pollingMtx; //mutex for _registered variable
		std::mutex responseMtx;

		int _currentTime = 0;
		int _interval = 20;
		int _motorSize = 20;
		HapticTimer timer;

		bool isRunning = false;

		bool _enable = false;

		std::string host = "127.0.0.1";
		int port = 15881;
		std::string path = "v2/feedbacks";

		int reconnectSec = 5;
		std::chrono::steady_clock::time_point prevReconnect;

		bool isRegisterSent = true;

		int connectionCount = 0;

		//functions

		void reconnect();

		void resendRegistered();

		bool connectionCheck();

		void send(PlayerRequest request);

		void updateActive(const std::string &key, const Frame& signal);

		void remove(const std::string &key);

		void removeAll();

		void callbackFunc();

	public:

		static	Position stringToPosition(const std::string deviceName);

		bool retryConnection = true;
		PlayerResponse CurrentResponse;

		void doRepeat();

		HapticPlayer() {};

		int registerFeedbackFromFile(const std::string &key, const std::string &filePath);

		int registerFeedbackFromString(const std::string &key, const std::string &jsonString);

		void init();

		void submit(const std::string &key, Position position, const std::vector<uint8_t> &motorBytes, int durationMillis);

		void submit(const std::string &key, Position position, const std::vector<DotPoint> &points, int durationMillis);

		void submit(const std::string &key, Position position, const std::vector<PathPoint> &points, int durationMillis);

		void submitRegistered(const std::string &key, const std::string &altKey, ScaleOption option, RotationOption rotOption);

		void submitRegistered(const std::string &key);

		bool isPlaying();

		bool isPlaying(const std::string &key);

		void turnOff();

		void turnOff(const std::string &key);

		void parseReceivedMessage(const char * message);

		void checkMessage();

		void destroy();

		void enableFeedback();

		void disableFeedback();

		void toggleFeedback();

		void parseResponse(PlayerResponse response);

		bool isDevicePlaying(Position device);

		bool isFeedbackRegistered(std::string key);

		bool anyFilesLoaded();

		std::vector<std::string> fileNames();

		void registerConnection(std::string Id);

		void unregisterConnection(std::string Id);

		std::map<std::string, std::vector<int>> getResponseStatus();

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