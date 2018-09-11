#if defined _WIN32 || defined _WIN64
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLIMPORT
#endif

#include "model.h"
#include <vector>

// Initialises a connection to the bHaptics Player. Should only be called once: when the game starts.
DLLIMPORT void Initialise();

// End the connecection to the bHaptics Player. Should only be called once: when the game ends.
DLLIMPORT void Destroy();

// Register a preset .tact feedback file, created using the bHaptics Designer.
// Registered files can then be called and played back using the given key.
// File is submitted as an already processed JSON string of the Project attribute.
DLLIMPORT void RegisterFeedback(std::string& Key, std::string& ProjectJson);

// Register a preset .tact feedback file, created using the bHaptics Designer.
// Registered files can then be called and played back using the given key.
// File Path is given, and feedback file is parsed and processed by the SDK.
DLLIMPORT void LoadAndRegisterFeedback(std::string& Key, std::string& FilePath);

// Submit a request to play a registered feedback file using its Key.
DLLIMPORT void SubmitRegistered(std::string& Key);

// Submit a request to play a registered feedback file, with additional options.
// ScaleOption scales the intensity and duration of the feedback by some factor.
// RotationOption uses cylindrical projection to rotate a Vest feedback file, as well as the vertical position.
// AltKey provides a unique key to play this custom feedback under, as opposed to the original feedback Key.
DLLIMPORT void SubmitRegisteredAlt(std::string& Key, std::string& AltKey, bhaptics::ScaleOption ScaleOpt, bhaptics::RotationOption RotOption);

// Submit an array of 20 integers, representing the strength of each motor vibration, ranging from 0 to 100.
// Specify the Position (playback device) as well as the duration of the feedback effect in milliseconds.
DLLIMPORT void Submit(std::string& Key, bhaptics::Position Pos, std::vector<uint8_t>& MotorBytes, int DurationMillis);

// Submit an array of DotPoints, representing the motor's Index and Intensity.
// Specify the Position (playback device) as well as the duration of the feedback effect in milliseconds.
DLLIMPORT void SubmitDot(std::string& Key, bhaptics::Position Pos, std::vector<bhaptics::DotPoint>& Points, int DurationMillis);

// Submit an array of PathPoints, representing the xy-position of the feedback on a unit square.
// Based off the intensity and position, specific motors are chosen by the bHaptics Player and vibrated.
// Specify the Position (playback device) as well as the duration of the feedback effect in milliseconds.
DLLIMPORT void SubmitPath(std::string& Key, bhaptics::Position Pos, std::vector<bhaptics::PathPoint>& Points, int DurationMillis);

// Boolean to check if a Feedback has been registered or not under the given Key.
DLLIMPORT bool IsFeedbackRegistered(std::string& key);

// Boolean to check if there are any Feedback effects currently playing.
DLLIMPORT bool IsPlaying();

// Boolean to check if a feedback effect under the given Key is currently playing.
DLLIMPORT bool IsPlayingKey(std::string& Key);

// Turn off all currently playing feedback effects.
DLLIMPORT void TurnOff();

// Turn off the feedback effect specified by the Key.
DLLIMPORT void TurnOffKey(std::string& Key);

// Enable Haptic Feedback calls to the bHaptics Player (on by default)
DLLIMPORT void EnableFeedback();

// Disable Haptic Feedback calls to the bHaptics Player
DLLIMPORT void DisableFeedback();

// Toggle between Enabling/Disabling haptic feedback.
DLLIMPORT void ToggleFeedback();

// Boolean to check if a specific device is connected to the bHaptics Player.
DLLIMPORT bool IsDevicePlaying(bhaptics::Position Pos);

// Returns an array of the current status of each device.
// Used for UI to ensure that haptic feedback is playing.
DLLIMPORT void GetResponseStatus(std::vector<bhaptics::HapticFeedback>& retValues);

// Returns the current motor values for a given device.
// Used for UI to ensure that haptic feedback is playing.
DLLIMPORT void GetResponseForPosition(std::vector<int>& retValues, std::string& pos);
