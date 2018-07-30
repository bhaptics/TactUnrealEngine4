#if defined _WIN32 || defined _WIN64
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLIMPORT
#endif

#include "SDK/model.h"
//#include "Json/Public/Dom/JsonObject.h"
#include <vector>

DLLIMPORT void ExampleLibraryFunction();

DLLIMPORT void RegisterConnection(std::string& Id);

DLLIMPORT void UnregisterConnection(std::string& Id);

DLLIMPORT void RegisterFeedback(std::string& Key, std::string& ProjectJson);

DLLIMPORT void SubmitRegistered(std::string& Key);

DLLIMPORT void SubmitRegisteredAlt(std::string& Key, std::string& AltKey, bhaptics::ScaleOption ScaleOpt, bhaptics::RotationOption RotOption);

DLLIMPORT void Submit(std::string& Key, bhaptics::Position Pos, std::vector<uint8_t>& MotorBytes, int DurationMillis);

DLLIMPORT void SubmitDot(std::string& Key, bhaptics::Position Pos, std::vector<bhaptics::DotPoint>& Points, int DurationMillis);

DLLIMPORT void SubmitPath(std::string& Key, bhaptics::Position Pos, std::vector<bhaptics::PathPoint>& Points, int DurationMillis);

DLLIMPORT bool IsFeedbackRegistered(std::string& key);

DLLIMPORT bool IsPlaying();

DLLIMPORT bool IsPlayingKey(std::string& Key);

DLLIMPORT void TurnOff();

DLLIMPORT void TurnOffKey(std::string& Key);

DLLIMPORT void EnableFeedback();

DLLIMPORT void DisableFeedback();

DLLIMPORT void ToggleFeedback();

DLLIMPORT bool IsDevicePlaying(bhaptics::Position Pos);

DLLIMPORT void GetResponseStatus(std::vector<bhaptics::HapticFeedback>& retValues);

DLLIMPORT void GetResponseForPosition(std::vector<int>& retValues, std::string& pos);
