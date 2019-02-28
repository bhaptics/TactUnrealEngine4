//Copyright bHaptics Inc. 2017 - 2019

#include "BhapticsLibrary.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"

#include "ThirdParty/HapticsManagerLibrary/HapticLibrary.h"

#include "HapticsManager.h"

bool BhapticsLibrary::IsInitialised = false;
bool BhapticsLibrary::IsLoaded = false;
FProcHandle BhapticsLibrary::Handle;
bool BhapticsLibrary::Success = false;

BhapticsLibrary::BhapticsLibrary()
{

}


BhapticsLibrary::~BhapticsLibrary()
{

}

void BhapticsLibrary::SetLibraryLoaded()
{
	IsLoaded = true;
}

bool BhapticsLibrary::InitialiseConnection()
{
	if (!IsLoaded)
	{
		return false;
	}

	if (IsInitialised)
	{
		return Success;
	}

	bool bLaunch = true;
	if (GConfig)
	{
		GConfig->GetBool(
			TEXT("/Script/HapticsManager.HapticSettings"),
			TEXT("bShouldLaunch"),
			bLaunch,
			GGameIni
		);
	}

	IsInitialised = true;

	FString ConfigPath = *FPaths::ProjectContentDir();
	ConfigPath.Append("/ConfigFiles/HapticPlayer.txt");
	if (FPaths::FileExists(ConfigPath)) {
		FString ExeLocation;
		FFileHelper::LoadFileToString(ExeLocation, *ConfigPath);
		if (FPaths::FileExists(ExeLocation)) {
			//if (!BhapticsUtilities::IsExternalPlayerRunning(ExeLocation))
			FString ExeName = FPaths::GetBaseFilename(*ExeLocation);

			if (!FPlatformProcess::IsApplicationRunning(*ExeName) && bLaunch)
			{
				Handle = FPlatformProcess::CreateProc(*ExeLocation, nullptr, true, true, false, nullptr, 0, nullptr, nullptr);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Could not launch: %s."), *ExeLocation);
		}
	}
	else
	{
		//FString ExePath(BhapticsUtilities::GetExecutablePath());
		FString ExePath(getExePath());

		if (!ExePath.IsEmpty() && FPaths::FileExists(ExePath))
		{
			FString ExeName = FPaths::GetBaseFilename(ExePath);
			UE_LOG(LogTemp, Log, TEXT("Exelocated: %s. %s"), *ExePath, *ExeName);
			UE_LOG(LogTemp, Log, TEXT("Player is installed"));

			if (!FPlatformProcess::IsApplicationRunning(*ExeName) && bLaunch)
			{
				UE_LOG(LogTemp, Log, TEXT("Player is not running - launching"));

				Handle = FPlatformProcess::CreateProc(*ExePath, nullptr, true, true, false, nullptr, 0, nullptr, nullptr);

			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Player is running"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Player is not Installed"));
			return false;
		}

	}

	Initialise();
	Success = true;
	return true;
}

void BhapticsLibrary::Free()
{
	if (!IsLoaded)
	{
		return;
	}

	Destroy();

	if (Handle.IsValid())
	{
		FPlatformProcess::TerminateProc(Handle);
		FPlatformProcess::CloseProc(Handle);
	}
}

void BhapticsLibrary::Lib_RegisterFeedback(FString Key, FString ProjectJson)
{
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	std::string ProjectString = (TCHAR_TO_UTF8(*ProjectJson));
	RegisterFeedback(StandardKey, ProjectString);
}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key)
{
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	SubmitRegistered(StandardKey);
}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption)
{
	if (!IsLoaded)
	{
		return;
	}
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
	if (!IsLoaded)
	{
		return;
	}
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
	case EPosition::ForearmL:
		HapticPosition = bhaptics::Position::ForearmL;
		break;
	case EPosition::ForearmR:
		HapticPosition = bhaptics::Position::ForearmR;
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
	if (!IsLoaded)
	{
		return;
	}
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
	case EPosition::ForearmL:
		HapticPosition = bhaptics::Position::ForearmL;
		break;
	case EPosition::ForearmR:
		HapticPosition = bhaptics::Position::ForearmR;
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
	if (!IsLoaded)
	{
		return;
	}
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
	case EPosition::ForearmL:
		HapticPosition = bhaptics::Position::ForearmL;
		break;
	case EPosition::ForearmR:
		HapticPosition = bhaptics::Position::ForearmR;
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
	if (!IsLoaded)
	{
		return false;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*key));
	bool Value = false;
	Value = IsFeedbackRegistered(StandardKey);
	return Value;
}

bool BhapticsLibrary::Lib_IsPlaying()
{
	if (!IsLoaded)
	{
		return false;
	}
	bool Value = false;
	Value = IsPlaying();
	return Value;
}

bool BhapticsLibrary::Lib_IsPlaying(FString Key)
{
	if (!IsLoaded)
	{
		return false;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	bool Value = false;
	Value = IsPlayingKey(StandardKey);
	return Value;
}

void BhapticsLibrary::Lib_TurnOff()
{
	if (!IsLoaded)
	{
		return;
	}
	TurnOff();
}

void BhapticsLibrary::Lib_TurnOff(FString Key)
{
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	TurnOffKey(StandardKey);
}

void BhapticsLibrary::Lib_EnableFeedback()
{
	if (!IsLoaded)
	{
		return;
	}
	EnableFeedback();
}

void BhapticsLibrary::Lib_DisableFeedback()
{
	if (!IsLoaded)
	{
		return;
	}
	DisableFeedback();
}

void BhapticsLibrary::Lib_ToggleFeedback()
{
	if (!IsLoaded)
	{
		return;
	}
	ToggleFeedback();
}

bool BhapticsLibrary::Lib_IsDevicePlaying(EPosition Pos)
{
	if (!IsLoaded)
	{
		return false;
	}
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
	case EPosition::ForearmL:
		device = bhaptics::Position::ForearmL;
		break;
	case EPosition::ForearmR:
		device = bhaptics::Position::ForearmR;
		break;
	case EPosition::VestFront:
		device = bhaptics::Position::Vest;
		break;
	case EPosition::VestBack:
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
	if (!IsLoaded)
	{
		return ChangedFeedbacks;
	}
	std::string Positions [] = {"ForearmL","ForearmR","Head", "VestFront", "VestBack", "HandL", "HandR", "FootL", "FootR"};
	TArray<EPosition> PositionEnum =
		{ EPosition::ForearmL,EPosition::ForearmR,EPosition::Head, EPosition::VestFront,EPosition::VestBack,EPosition::HandL, EPosition::HandR, EPosition::FootL, EPosition::FootR };

	for (int i = 0; i < 9; i++)
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

