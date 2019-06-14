// Copyright bHaptics Inc. 2017-2019

#include "HapticPlayer.h"
#include "HapticLibrary/HapticLibrary.h"
#include "Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"

bool HapticPlayer::IsInitialised = false;
FProcHandle HapticPlayer::Handle;
bool HapticPlayer::Success = false;

bool HapticPlayer::Initialise()
{
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
		char Path[100];
		int Size = 0;
		bool Result = TryGetExePath(Path, Size);
		FString ExePath(Path);
		if (Result)
		{
			if (!ExePath.IsEmpty() && FPaths::FileExists(ExePath))
			{
				FString ExeName = FPaths::GetBaseFilename(ExePath);
				UE_LOG(LogTemp, Log, TEXT("Exelocated: %s."), *ExePath);
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
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Registry check failed - Initialising"));
		}

	}
	Initialise();
	Success = true;
	return true;
}

void HapticPlayer::Destroy() 
{
	Destroy();
	if (Handle.IsValid())
	{
		FPlatformProcess::TerminateProc(Handle);
		FPlatformProcess::CloseProc(Handle);
	}
}

void HapticPlayer::RegisterFeedback(FString Key, FString ProjectJson) {
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	std::string ProjectString = (TCHAR_TO_UTF8(*ProjectJson));
	RegisterFeedback(StandardKey.c_str(), ProjectString.c_str());
}

void HapticPlayer::SubmitRegistered(FString Key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	SubmitRegistered(StandardKey.c_str());
}

void HapticPlayer::SubmitRegistered(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	std::string StandardAltKey(TCHAR_TO_UTF8(*AltKey));
	bhaptics::RotationOption RotateOption;
	bhaptics::ScaleOption Option;
	RotateOption.OffsetAngleX = RotOption.OffsetAngleX;
	RotateOption.OffsetY = RotOption.OffsetY;

	Option.Intensity = ScaleOpt.Intensity;
	Option.Duration = ScaleOpt.Duration;
	SubmitRegisteredAlt(StandardKey.c_str(), StandardAltKey.c_str(), Option, RotateOption);
}

void HapticPlayer::SubmitFeedback(FString Key, EPosition Pos, TArray<uint8> MotorBytes, int DurationMillis)
{
	bhaptics::PositionType HapticPosition = bhaptics::PositionType::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));

	switch (Pos)
	{
	case EPosition::Left:
		HapticPosition = bhaptics::PositionType::Left;
		break;
	case EPosition::Right:
		HapticPosition = bhaptics::PositionType::Right;
		break;
	case EPosition::Head:
		HapticPosition = bhaptics::PositionType::Head;
		break;
	case EPosition::VestFront:
		HapticPosition = bhaptics::PositionType::VestFront;
		break;
	case EPosition::VestBack:
		HapticPosition = bhaptics::PositionType::VestBack;
		break;
	case EPosition::HandL:
		HapticPosition = bhaptics::PositionType::HandL;
		break;
	case EPosition::HandR:
		HapticPosition = bhaptics::PositionType::HandR;
		break;
	case EPosition::FootL:
		HapticPosition = bhaptics::PositionType::FootL;
		break;
	case EPosition::FootR:
		HapticPosition = bhaptics::PositionType::FootR;
		break;
	case EPosition::ForearmL:
		HapticPosition = bhaptics::PositionType::ForearmL;
		break;
	case EPosition::ForearmR:
		HapticPosition = bhaptics::PositionType::ForearmR;
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

	Submit(StandardKey.c_str(), HapticPosition, SubmittedDots, DurationMillis);
}

void HapticPlayer::SubmitFeedback(FString Key, EPosition Pos, TArray<FDotPoint> Points, int DurationMillis)
{
	bhaptics::PositionType HapticPosition = bhaptics::PositionType::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	switch (Pos)
	{
	case EPosition::Left:
		HapticPosition = bhaptics::PositionType::Left;
		break;
	case EPosition::Right:
		HapticPosition = bhaptics::PositionType::Right;
		break;
	case EPosition::Head:
		HapticPosition = bhaptics::PositionType::Head;
		break;
	case EPosition::VestFront:
		HapticPosition = bhaptics::PositionType::VestFront;
		break;
	case EPosition::VestBack:
		HapticPosition = bhaptics::PositionType::VestBack;
		break;
	case EPosition::HandL:
		HapticPosition = bhaptics::PositionType::HandL;
		break;
	case EPosition::HandR:
		HapticPosition = bhaptics::PositionType::HandR;
		break;
	case EPosition::FootL:
		HapticPosition = bhaptics::PositionType::FootL;
		break;
	case EPosition::FootR:
		HapticPosition = bhaptics::PositionType::FootR;
		break;
	case EPosition::ForearmL:
		HapticPosition = bhaptics::PositionType::ForearmL;
		break;
	case EPosition::ForearmR:
		HapticPosition = bhaptics::PositionType::ForearmR;
		break;
	default:
		break;
	}

	std::vector<bhaptics::DotPoint> SubmittedDots;

	for (int32 i = 0; i < Points.Num(); i++)
	{
		SubmittedDots.push_back(bhaptics::DotPoint(Points[i].Index, Points[i].Intensity));
	}

	SubmitDot(StandardKey.c_str(), HapticPosition, SubmittedDots, DurationMillis);
}

void HapticPlayer::SubmitFeedback(FString Key, EPosition Pos, TArray<FPathPoint> Points, int DurationMillis)
{
	bhaptics::PositionType HapticPosition = bhaptics::PositionType::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	switch (Pos)
	{
	case EPosition::Left:
		HapticPosition = bhaptics::PositionType::Left;
		break;
	case EPosition::Right:
		HapticPosition = bhaptics::PositionType::Right;
		break;
	case EPosition::Head:
		HapticPosition = bhaptics::PositionType::Head;
		break;
	case EPosition::VestFront:
		HapticPosition = bhaptics::PositionType::VestFront;
		break;
	case EPosition::VestBack:
		HapticPosition = bhaptics::PositionType::VestBack;
		break;
	case EPosition::HandL:
		HapticPosition = bhaptics::PositionType::HandL;
		break;
	case EPosition::HandR:
		HapticPosition = bhaptics::PositionType::HandR;
		break;
	case EPosition::FootL:
		HapticPosition = bhaptics::PositionType::FootL;
		break;
	case EPosition::FootR:
		HapticPosition = bhaptics::PositionType::FootR;
		break;
	case EPosition::ForearmL:
		HapticPosition = bhaptics::PositionType::ForearmL;
		break;
	case EPosition::ForearmR:
		HapticPosition = bhaptics::PositionType::ForearmR;
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

	SubmitPath(StandardKey.c_str(), HapticPosition, PathVector, DurationMillis);
}

void HapticPlayer::TurnOff()
{
	TurnOff();
}

void HapticPlayer::TurnOff(FString Key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	TurnOffKey(StandardKey.c_str());
}

bool HapticPlayer::IsRegistered(FString key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*key));
	bool Value = false;
	Value = IsFeedbackRegistered(StandardKey.c_str());
	return Value;
}

bool HapticPlayer::IsPlaying()
{
	bool Value = false;
	Value = IsPlaying();
	return Value;
}

bool HapticPlayer::IsPlaying(FString Key)
{

	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	bool Value = false;
	Value = IsPlayingKey(StandardKey.c_str());
	return Value;
}

bool HapticPlayer::IsDeviceConnected(EPosition Pos)
{
	bhaptics::PositionType device = bhaptics::PositionType::All;

		switch (Pos)
		{
		case EPosition::Left:
			device = bhaptics::PositionType::Left;
			break;
		case EPosition::Right:
			device = bhaptics::PositionType::Right;
			break;
		case EPosition::Head:
			device = bhaptics::PositionType::Head;
			break;
		case EPosition::HandL:
			device = bhaptics::PositionType::HandL;
			break;
		case EPosition::HandR:
			device = bhaptics::PositionType::HandR;
			break;
		case EPosition::FootL:
			device = bhaptics::PositionType::FootL;
			break;
		case EPosition::FootR:
			device = bhaptics::PositionType::FootR;
			break;
		case EPosition::ForearmL:
			device = bhaptics::PositionType::ForearmL;
			break;
		case EPosition::ForearmR:
			device = bhaptics::PositionType::ForearmR;
			break;
		case EPosition::VestFront:
			device = bhaptics::PositionType::Vest;
			break;
		case EPosition::VestBack:
			device = bhaptics::PositionType::Vest;
			break;
		default:
			return false;
			break;
		}

	bool Value = false;
	Value = IsDevicePlaying(device);
	return Value;
}

TArray<FHapticFeedback> HapticPlayer::GetResponseStatus()
{
	TArray<FHapticFeedback> ChangedFeedbacks;
	TArray<bhaptics::PositionType> Positions =
		{ bhaptics::PositionType::ForearmL,bhaptics::PositionType::ForearmR,bhaptics::PositionType::Head, bhaptics::PositionType::VestFront,bhaptics::PositionType::VestBack,
		bhaptics::PositionType::HandL, bhaptics::PositionType::HandR, bhaptics::PositionType::FootL, bhaptics::PositionType::FootR };
	TArray<EPosition> PositionEnum =
		{ EPosition::ForearmL,EPosition::ForearmR,EPosition::Head, EPosition::VestFront,EPosition::VestBack,EPosition::HandL, EPosition::HandR, EPosition::FootL, EPosition::FootR };

	for (int i = 0; i < Positions.Num(); i++)
	{
		status stat;
		bool res = TryGetResponseForPosition(Positions[i], stat);
		TArray<uint8> val;
		val.Init(0, 20);
		if (res)
		{
			for (int j = 0; j < val.Num(); j++)
			{
				val[j] = stat.values[j];
			}
		}

		FHapticFeedback Feedback = FHapticFeedback(PositionEnum[i], val, EFeedbackMode::DOT_MODE);
		ChangedFeedbacks.Add(Feedback);
	}

	return ChangedFeedbacks;
}

void HapticPlayer::EnableFeedback()
{
	EnableFeedback();
}

void HapticPlayer::DisableFeedback()
{
	DisableFeedback();
}

void HapticPlayer::ToggleFeedback()
{
	ToggleFeedback();
}

void HapticPlayer::startScan()
{
}

void HapticPlayer::stopScan()
{
}

