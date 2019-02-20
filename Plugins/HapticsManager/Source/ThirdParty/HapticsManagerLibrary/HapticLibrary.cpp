#if defined _WIN32 || defined _WIN64
    #include <Windows.h>

    #define DLLEXPORT __declspec(dllexport)
#else
    #include <stdio.h>
#endif

#include "hapticsManager.h"
#include "model.h"
#ifndef DLLEXPORT
    #define DLLEXPORT
#endif

static bool isInit = false;
static const char* exeFilePath = "";

//char* getExePath
DLLEXPORT const char* getExePath()
{
	if (isInit)
	{
		return exeFilePath;
	}
	isInit = true;
#if defined _WIN32 || defined _WIN64
	HKEY hKey;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	LONG resultFirstKey = RegOpenKeyExW(HKEY_CLASSES_ROOT, L"bhaptics-app\\shell\\open\\command", 0, KEY_READ, &hKey);
	LONG result = RegQueryValueEx(hKey, L"", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	RegCloseKey(hKey);
	if (result == ERROR_SUCCESS)
	{
		std::wstring path = szBuffer;
		int size = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, NULL, 0, NULL, NULL);

		char *buffer = new char[size + 1];
		WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, buffer, size, NULL, NULL);

		std::string str(buffer);
		delete[]buffer;
		exeFilePath = str.c_str();

		return exeFilePath;
	}

	return "";
#else
	return "";
#endif
}

DLLEXPORT void Initialise()
{
	bhaptics::HapticPlayer::instance()->registerConnection("Plugin");
}

DLLEXPORT void Destroy()
{
	//bhaptics::HapticPlayer::instance()->unregisterConnection("Plugin");
	bhaptics::HapticPlayer::instance()->destroy();
}

DLLEXPORT void RegisterFeedback(std::string& Key, std::string& ProjectJson)
{
	bhaptics::HapticPlayer::instance()->registerFeedbackFromString(Key, ProjectJson);
}

DLLEXPORT void LoadAndRegisterFeedback(std::string& Key, std::string& FilePath)
{
	bhaptics::HapticPlayer::instance()->registerFeedbackFromFile(Key,FilePath);
}

DLLEXPORT void SubmitRegistered(std::string& Key)
{
	bhaptics::HapticPlayer::instance()->submitRegistered(Key);
}

DLLEXPORT void SubmitRegisteredAlt(std::string& Key, std::string& AltKey, bhaptics::ScaleOption ScaleOpt, bhaptics::RotationOption RotOption)
{
	bhaptics::HapticPlayer::instance()->submitRegistered(Key, AltKey, ScaleOpt, RotOption);
}

DLLEXPORT void Submit(std::string& Key, bhaptics::Position Pos, std::vector<uint8_t>& MotorBytes, int DurationMillis)
{
	bhaptics::HapticPlayer::instance()->submit(Key, Pos, MotorBytes, DurationMillis);
}

DLLEXPORT void SubmitDot(std::string& Key, bhaptics::Position Pos, std::vector<bhaptics::DotPoint>& Points, int DurationMillis)
{
	bhaptics::HapticPlayer::instance()->submit(Key, Pos, Points, DurationMillis);
}

DLLEXPORT void SubmitPath(std::string& Key, bhaptics::Position Pos, std::vector<bhaptics::PathPoint>& Points, int DurationMillis)
{
	bhaptics::HapticPlayer::instance()->submit(Key, Pos, Points, DurationMillis);
}

DLLEXPORT bool IsFeedbackRegistered(std::string& key)
{
	return bhaptics::HapticPlayer::instance()->isFeedbackRegistered(key);
}

DLLEXPORT bool IsPlaying()
{
	return bhaptics::HapticPlayer::instance()->isPlaying();
}

DLLEXPORT bool IsPlayingKey(std::string& Key)
{
	return bhaptics::HapticPlayer::instance()->isPlaying(Key);
}

DLLEXPORT void TurnOff()
{
	bhaptics::HapticPlayer::instance()->turnOff();
}

DLLEXPORT void TurnOffKey(std::string& Key)
{
	bhaptics::HapticPlayer::instance()->turnOff(Key);
}

DLLEXPORT void EnableFeedback()
{
	bhaptics::HapticPlayer::instance()->enableFeedback();
}

DLLEXPORT void DisableFeedback()
{
	bhaptics::HapticPlayer::instance()->disableFeedback();
}

DLLEXPORT void ToggleFeedback()
{
	bhaptics::HapticPlayer::instance()->toggleFeedback();
}

DLLEXPORT bool IsDevicePlaying(bhaptics::Position Pos)
{
	return bhaptics::HapticPlayer::instance()->isDevicePlaying(Pos);
}

DLLEXPORT void GetResponseForPosition(std::vector<int>& retValues, std::string& pos)
{
	if (!bhaptics::HapticPlayer::instance()->getResponseStatus().empty())
	{
		std::map<std::string, std::vector<int>> responseMap = bhaptics::HapticPlayer::instance()->getResponseStatus();
		if (responseMap.find(pos) != responseMap.end())
		{
			std::vector<int> response = responseMap.at(pos);//bhaptics::HapticPlayer::instance()->getResponseStatus().at(pos);
			/*retValues = response.at(pos);*/
			for (int i = 0; i < retValues.size(); i++)
			{
				retValues[i] = response.at(i);
			}
		}
	}
}

DLLEXPORT void GetResponseStatus(std::vector<bhaptics::HapticFeedback>& retValues)
{
	std::map<std::string, std::vector<int>> response = bhaptics::HapticPlayer::instance()->getResponseStatus();
	for (auto& Device : response)
	{
		bhaptics::Position Position = bhaptics::Position::ForearmL;

		if (Device.first == "Left")
		{
			Position = bhaptics::Position::Left;
		}
		else if (Device.first == "Right")
		{
			Position = bhaptics::Position::Right;
		}
		if (Device.first == "ForearmL")
		{
			Position = bhaptics::Position::ForearmL;
		}
		else if (Device.first == "ForearmR")
		{
			Position = bhaptics::Position::ForearmR;
		}
		else if (Device.first == "Head")
		{
			Position = bhaptics::Position::Head;
		}
		else if (Device.first == "VestFront")
		{
			Position = bhaptics::Position::VestFront;
		}
		else if (Device.first == "VestBack")
		{
			Position = bhaptics::Position::VestBack;
		}
		else if (Device.first == "Racket")
		{
			Position = bhaptics::Position::Racket;
		}
		else if (Device.first == "HandL")
		{
			Position = bhaptics::Position::HandL;
		}
		else if (Device.first == "HandR")
		{
			Position = bhaptics::Position::HandR;
		}
		else if (Device.first == "FootL")
		{
			Position = bhaptics::Position::FootL;
		}
		else if (Device.first == "FootR")
		{
			Position = bhaptics::Position::FootR;
		}
		else
		{
			continue;
		}

		//bhaptics::HapticFeedback Feedback;
		//Feedback.DevicePosition = Position;
		//Feedback.Values = Device.second;
		//retValues.push_back(Feedback);
	}
}