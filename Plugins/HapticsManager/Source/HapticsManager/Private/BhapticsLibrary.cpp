//Copyright bHaptics Inc. 2018

#if (ENGINE_MINOR_VERSION < 16)
//#include "HapticsManager.h"
#endif
#include "BhapticsLibrary.h"
#include "Interfaces/IPluginManager.h"

#if (ENGINE_MINOR_VERSION >= 16)
#include "Core/Public/Misc/Paths.h"
#endif

#include "ThirdParty/HapticsManagerLibrary/HapticLibrary.h"
#include "HapticsManager.h"

bool BhapticsLibrary::IsInitialised = false;

void *v_dllLibraryHandle;

BhapticsLibrary::BhapticsLibrary()
{

}


BhapticsLibrary::~BhapticsLibrary()
{

}

bool BhapticsLibrary::InitialiseConnection()
{
	Initialise();
	return true;
}

void BhapticsLibrary::Free()
{
	Destroy();
}

void BhapticsLibrary::Lib_RegisterFeedback(FString Key, FString ProjectJson)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	std::string ProjectString = (TCHAR_TO_UTF8(*ProjectJson));
	RegisterFeedback(StandardKey, ProjectString);
}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	SubmitRegistered(StandardKey);
}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	std::string StandardAltKey(TCHAR_TO_UTF8(*AltKey));
	bhaptics::RotationOption RotateOption;
	bhaptics::ScaleOption Option;
	RotateOption.OffsetAngleX = RotOption.OffsetAngleX;
	RotateOption.OffsetY = RotOption.OffsetY;

	Option.Intensity = ScaleOpt.Intensity;
	Option.Duration = ScaleOpt.Duration;
	SubmitRegisteredAlt(StandardKey, StandardAltKey, Option, RotateOption);
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<uint8> MotorBytes, int DurationMillis)
{
	bhaptics::Position HapticPosition = bhaptics::Position::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));

	switch (Pos)
	{
	case EPosition::Left:
		HapticPosition = bhaptics::Position::Left;
		break;
	case EPosition::Right:
		HapticPosition = bhaptics::Position::Right;
		break;
	case EPosition::Head:
		HapticPosition = bhaptics::Position::Head;
		break;
	case EPosition::VestFront:
		HapticPosition = bhaptics::Position::VestFront;
		break;
	case EPosition::VestBack:
		HapticPosition = bhaptics::Position::VestBack;
		break;
	case EPosition::HandL:
		HapticPosition = bhaptics::Position::HandL;
		break;
	case EPosition::HandR:
		HapticPosition = bhaptics::Position::HandR;
		break;
	case EPosition::FootL:
		HapticPosition = bhaptics::Position::FootL;
		break;
	case EPosition::FootR:
		HapticPosition = bhaptics::Position::FootR;
		break;
	case EPosition::Racket:
		HapticPosition = bhaptics::Position::Racket;
		break;
	default:
		break;
	}

	if (MotorBytes.Num() != 20)
	{
		printf("Invalid Point Array\n");
		return;
	}

	std::vector<uint8_t> SubmittedDots(20, 0);

	for (int32 i = 0; i < MotorBytes.Num(); i++)
	{
		SubmittedDots[i] = MotorBytes[i];
	}

	Submit(StandardKey, HapticPosition, SubmittedDots, DurationMillis);
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<FDotPoint> Points, int DurationMillis)
{
	bhaptics::Position HapticPosition = bhaptics::Position::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	switch (Pos)
	{
	case EPosition::Left:
		HapticPosition = bhaptics::Position::Left;
		break;
	case EPosition::Right:
		HapticPosition = bhaptics::Position::Right;
		break;
	case EPosition::Head:
		HapticPosition = bhaptics::Position::Head;
		break;
	case EPosition::VestFront:
		HapticPosition = bhaptics::Position::VestFront;
		break;
	case EPosition::VestBack:
		HapticPosition = bhaptics::Position::VestBack;
		break;
	case EPosition::HandL:
		HapticPosition = bhaptics::Position::HandL;
		break;
	case EPosition::HandR:
		HapticPosition = bhaptics::Position::HandR;
		break;
	case EPosition::FootL:
		HapticPosition = bhaptics::Position::FootL;
		break;
	case EPosition::FootR:
		HapticPosition = bhaptics::Position::FootR;
		break;
	case EPosition::Racket:
		HapticPosition = bhaptics::Position::Racket;
		break;
	default:
		break;
	}

	std::vector<bhaptics::DotPoint> SubmittedDots;

	for (int32 i = 0; i < Points.Num(); i++)
	{
		SubmittedDots.push_back(bhaptics::DotPoint(Points[i].Index, Points[i].Intensity));
	}

	SubmitDot(StandardKey, HapticPosition, SubmittedDots, DurationMillis);
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<FPathPoint> Points, int DurationMillis)
{
	bhaptics::Position HapticPosition = bhaptics::Position::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	switch (Pos)
	{
	case EPosition::Left:
		HapticPosition = bhaptics::Position::Left;
		break;
	case EPosition::Right:
		HapticPosition = bhaptics::Position::Right;
		break;
	case EPosition::Head:
		HapticPosition = bhaptics::Position::Head;
		break;
	case EPosition::VestFront:
		HapticPosition = bhaptics::Position::VestFront;
		break;
	case EPosition::VestBack:
		HapticPosition = bhaptics::Position::VestBack;
		break;
	case EPosition::HandL:
		HapticPosition = bhaptics::Position::HandL;
		break;
	case EPosition::HandR:
		HapticPosition = bhaptics::Position::HandR;
		break;
	case EPosition::FootL:
		HapticPosition = bhaptics::Position::FootL;
		break;
	case EPosition::FootR:
		HapticPosition = bhaptics::Position::FootR;
		break;
	case EPosition::Racket:
		HapticPosition = bhaptics::Position::Racket;
		break;
	default:
		break;
	}

	std::vector<bhaptics::PathPoint> PathVector;

	for (int32 i = 0; i < Points.Num(); i++)
	{
		int XVal = Points[i].X*1000;
		int YVal = Points[i].Y*1000;
		bhaptics::PathPoint Point(XVal, YVal, Points[i].Intensity, Points[i].MotorCount);
		PathVector.push_back(Point);
	}

	SubmitPath(StandardKey, HapticPosition, PathVector, DurationMillis);
}

bool BhapticsLibrary::Lib_IsFeedbackRegistered(FString key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*key));
	bool Value = false;
	Value = IsFeedbackRegistered(StandardKey);
	return Value;
}

bool BhapticsLibrary::Lib_IsPlaying()
{
	bool Value = false;
	Value = IsPlaying();
	return Value;
}

bool BhapticsLibrary::Lib_IsPlaying(FString Key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	bool Value = false;
	Value = IsPlayingKey(StandardKey);
	return Value;
}

void BhapticsLibrary::Lib_TurnOff()
{
	TurnOff();
}

void BhapticsLibrary::Lib_TurnOff(FString Key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	TurnOffKey(StandardKey);
}

void BhapticsLibrary::Lib_EnableFeedback()
{
	EnableFeedback();
}

void BhapticsLibrary::Lib_DisableFeedback()
{
	DisableFeedback();
}

void BhapticsLibrary::Lib_ToggleFeedback()
{
	ToggleFeedback();
}

bool BhapticsLibrary::Lib_IsDevicePlaying(EPosition Pos)
{
	bhaptics::Position device = bhaptics::Position::All;

	switch (Pos)
	{
	case EPosition::Left:
		device = bhaptics::Position::Left;
		break;
	case EPosition::Right:
		device = bhaptics::Position::Right;
		break;
	case EPosition::Head:
		device = bhaptics::Position::Head;
		break;
	case EPosition::Racket:
		device = bhaptics::Position::Racket;
		break;
	case EPosition::HandL:
		device = bhaptics::Position::HandL;
		break;
	case EPosition::HandR:
		device = bhaptics::Position::HandR;
		break;
	case EPosition::FootL:
		device = bhaptics::Position::FootL;
		break;
	case EPosition::FootR:
		device = bhaptics::Position::FootR;
		break;
	case EPosition::VestFront:
		device = bhaptics::Position::Vest;
		break;
	case EPosition::VestBack:
		device = bhaptics::Position::Vest;
		break;
	case EPosition::Vest:
		device = bhaptics::Position::Vest;
		break;
	default:
		return false;
		break;
	}

	bool Value = false;
	Value = IsDevicePlaying(device);
	return Value;
}

TArray<FHapticFeedback> BhapticsLibrary::Lib_GetResponseStatus()
{
	TArray<FHapticFeedback> ChangedFeedbacks;
	std::string Positions [] = {"Left","Right","Head", "VestFront", "VestBack", "Racket", "HandL", "HandR", "FootL", "FootR"};
	TArray<EPosition> PositionEnum =
		{ EPosition::Left,EPosition::Right,EPosition::Head,EPosition::VestFront,EPosition::VestBack,EPosition::Racket,EPosition::HandL, EPosition::HandR, EPosition::FootL, EPosition::FootR };

	for (int i = 0; i < 10; i++)
	{
		std::vector<int> values;
		values.resize(20);
		GetResponseForPosition(values, Positions[i]);
		TArray<uint8> val;
		for (size_t j = 0; j < values.size(); j++)
		{
			val.Add(values[j]);
		}

		FHapticFeedback Feedback = FHapticFeedback(PositionEnum[i], val, EFeedbackMode::DOT_MODE);
		ChangedFeedbacks.Add(Feedback);
	}

	return ChangedFeedbacks;
}

