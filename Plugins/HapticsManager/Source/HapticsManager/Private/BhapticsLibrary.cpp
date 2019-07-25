//Copyright bHaptics Inc. 2017 - 2019

#include "BhapticsLibrary.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"


#if PLATFORM_ANDROID
#include "AndroidHapticLibrary.h"
#else
#include "HapticLibrary/HapticLibrary.h"
#endif // PLATFORM_ANDROID

#include "HapticsManager.h"

bool BhapticsLibrary::IsInitialised = false;
bool BhapticsLibrary::IsLoaded = false;
FProcHandle BhapticsLibrary::Handle;
bool BhapticsLibrary::Success = false;

#if PLATFORM_ANDROID
#else
static bhaptics::PositionType PositionEnumToPositionType(EPosition Position);
#endif

static FString PositionEnumToString(EPosition Position);

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
#if PLATFORM_ANDROID
#else
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
		char Path [100];
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
#endif // !PLATFORM_ANDROID
	return true;
}

void BhapticsLibrary::Free()
{
#if PLATFORM_ANDROID

	UAndroidHapticLibrary::AndroidThunkCpp_StopScanning();
#else
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
#endif // PLATFORM_ANDROID

}

void BhapticsLibrary::Lib_RegisterFeedback(FString Key, FString ProjectJson)
{
#if PLATFORM_ANDROID
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ProjectJson);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Deserialisation failed."));
		return;
	}
	FRegisterRequest RegisterRequest = FRegisterRequest();
	RegisterRequest.Key = Key;
	RegisterRequest.Project = JsonObject;
	UAndroidHapticLibrary::SubmitRequestToPlayer(RegisterRequest);
#else
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	std::string ProjectString = (TCHAR_TO_UTF8(*ProjectJson));
	RegisterFeedback(StandardKey.c_str(), ProjectString.c_str());
#endif
}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key)
{
#if PLATFORM_ANDROID
	FSubmitRequest Request = FSubmitRequest();
	Request.Key = Key;
	Request.Type = "key";
	UAndroidHapticLibrary::SubmitRequestToPlayer(Request);
#else
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	SubmitRegistered(StandardKey.c_str());
#endif // PLATFORM_ANDROID

}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption)
{
#if PLATFORM_ANDROID
	FSubmitRequest Request = FSubmitRequest();
	Request.Key = Key;
	Request.Type = "key";
	Request.Parameters.Add("rotationOption", RotOption.ToString());
	Request.Parameters.Add("scaleOption", ScaleOpt.ToString());
	Request.Parameters.Add("altKey", AltKey);
	UAndroidHapticLibrary::SubmitRequestToPlayer(Request);
#else
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
	SubmitRegisteredAlt(StandardKey.c_str(), StandardAltKey.c_str(), Option, RotateOption);
#endif // PLATFORM_ANDROID
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<uint8> MotorBytes, int DurationMillis)
{
#if PLATFORM_ANDROID
	TArray<FDotPoint> Points = TArray<FDotPoint>();
	for (int i = 0; i < MotorBytes.Num(); i++)
	{
		if (MotorBytes[i] > 0)
		{
			Points.Add(FDotPoint(i, MotorBytes[i]));
		}
	}
	Lib_Submit(Key, Pos, Points, DurationMillis);
#else
	if (!IsLoaded)
	{
		return;
	}

	bhaptics::PositionType HapticPosition = PositionEnumToPositionType(Pos);
	std::string StandardKey(TCHAR_TO_UTF8(*Key));

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
#endif // PLATFORM_ANDROID
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<FDotPoint> Points, int DurationMillis)
{
#if PLATFORM_ANDROID
	FHapticFrame SubmissionFrame = FHapticFrame();
	SubmissionFrame.DotPoints = Points;
	SubmissionFrame.Position = PositionEnumToString(Pos);
	SubmissionFrame.PathPoints = TArray<FPathPoint>();
	SubmissionFrame.DurationMillis = DurationMillis;
	UAndroidHapticLibrary::SubmitFrame(Key, SubmissionFrame);
#else
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	bhaptics::PositionType HapticPosition = PositionEnumToPositionType(Pos);

	std::vector<bhaptics::DotPoint> SubmittedDots;

	for (int32 i = 0; i < Points.Num(); i++)
	{
		SubmittedDots.push_back(bhaptics::DotPoint(Points[i].Index, Points[i].Intensity));
	}

	SubmitDot(StandardKey.c_str(), HapticPosition, SubmittedDots, DurationMillis);
#endif // PLATFORM_ANDROID
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<FPathPoint> Points, int DurationMillis)
{
#if PLATFORM_ANDROID
	FHapticFrame SubmissionFrame = FHapticFrame();
	SubmissionFrame.DotPoints = TArray<FDotPoint>();
	SubmissionFrame.Position = PositionEnumToString(Pos);
	SubmissionFrame.PathPoints = Points;
	SubmissionFrame.DurationMillis = DurationMillis;
	UAndroidHapticLibrary::SubmitFrame(Key, SubmissionFrame);
#else
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	bhaptics::PositionType HapticPosition = PositionEnumToPositionType(Pos);

	std::vector<bhaptics::PathPoint> PathVector;

	for (int32 i = 0; i < Points.Num(); i++)
	{
		int XVal = Points[i].X*1000;
		int YVal = Points[i].Y*1000;
		bhaptics::PathPoint Point(XVal, YVal, Points[i].Intensity, Points[i].MotorCount);
		PathVector.push_back(Point);
	}

	SubmitPath(StandardKey.c_str(), HapticPosition, PathVector, DurationMillis);
#endif
}

bool BhapticsLibrary::Lib_IsFeedbackRegistered(FString Key)
{
	bool Value = false;
#if PLATFORM_ANDROID
	FPlayerResponse Response = UAndroidHapticLibrary::GetCurrentResponse();
	Value = Response.RegisteredKeys.Find(Key) != INDEX_NONE;
#else
	if (!IsLoaded)
	{
		return false;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	Value = IsFeedbackRegistered(StandardKey.c_str());
#endif
	return Value;
}

bool BhapticsLibrary::Lib_IsPlaying()
{
	bool Value = false;
#if PLATFORM_ANDROID
	FPlayerResponse Response = UAndroidHapticLibrary::GetCurrentResponse();
	Value = Response.ActiveKeys.Num() > 0;
#else
	if (!IsLoaded)
	{
		return false;
	}
	Value = IsPlaying();
#endif
	return Value;
}

bool BhapticsLibrary::Lib_IsPlaying(FString Key)
{
	bool Value = false;
#if PLATFORM_ANDROID
	FPlayerResponse Response = UAndroidHapticLibrary::GetCurrentResponse();
	Value = Response.ActiveKeys.Find(Key) != INDEX_NONE;
#else
	if (!IsLoaded)
	{
		return false;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	Value = IsPlayingKey(StandardKey.c_str());
#endif
	return Value;
}

void BhapticsLibrary::Lib_TurnOff()
{
#if PLATFORM_ANDROID
	FSubmitRequest Request = FSubmitRequest();
	Request.Key = "";
	Request.Type = "turnOffAll";
	UAndroidHapticLibrary::SubmitRequestToPlayer(Request);
#else
	if (!IsLoaded)
	{
		return;
	}
	TurnOff();
#endif // PLATFORM_ANDROID
}

void BhapticsLibrary::Lib_TurnOff(FString Key)
{
#if PLATFORM_ANDROID
	FSubmitRequest Request = FSubmitRequest();
	Request.Key = Key;
	Request.Type = "turnOff";
	UAndroidHapticLibrary::SubmitRequestToPlayer(Request);
#else
	if (!IsLoaded)
	{
		return;
	}
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	TurnOffKey(StandardKey.c_str());
#endif
}

void BhapticsLibrary::Lib_EnableFeedback()
{
#if PLATFORM_ANDROID
#else
	if (!IsLoaded)
	{
		return;
	}
	EnableFeedback();
#endif
}

void BhapticsLibrary::Lib_DisableFeedback()
{
#if PLATFORM_ANDROID
#else
	if (!IsLoaded)
	{
		return;
	}
	DisableFeedback();
#endif
}

void BhapticsLibrary::Lib_ToggleFeedback()
{
#if PLATFORM_ANDROID
#else
	if (!IsLoaded)
	{
		return;
	}
	ToggleFeedback();
#endif
}

bool BhapticsLibrary::Lib_IsDevicePlaying(EPosition Pos)
{
	bool Value = false;
#if PLATFORM_ANDROID
	FString DeviceString = PositionEnumToString(Pos);
	FPlayerResponse Response = UAndroidHapticLibrary::GetCurrentResponse();
	Value = Response.ConnectedPositions.Find(DeviceString) != INDEX_NONE;
#else
	if (!IsLoaded)
	{
		return false;
	}
	bhaptics::PositionType device = PositionEnumToPositionType(Pos);
	Value = IsDevicePlaying(device);
#endif
	return Value;
}

TArray<FHapticFeedback> BhapticsLibrary::Lib_GetResponseStatus()
{
	TArray<FHapticFeedback> ChangedFeedback;
#if PLATFORM_ANDROID
	FPlayerResponse Response = UAndroidHapticLibrary::GetCurrentResponse();
	ChangedFeedback = Response.Status;
#else
	if (!IsLoaded)
	{
		return ChangedFeedback;
	}
	//std::string Positions [] = {"ForearmL","ForearmR","Head", "VestFront", "VestBack", "HandL", "HandR", "FootL", "FootR"};
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
		ChangedFeedback.Add(Feedback);
	}
#endif
	return ChangedFeedback;
}

#if !PLATFORM_ANDROID
static bhaptics::PositionType PositionEnumToPositionType(EPosition Position)
{
	bhaptics::PositionType Device = bhaptics::PositionType::All;

	switch (Position)
	{
	case EPosition::Left:
		Device = bhaptics::PositionType::Left;
		break;
	case EPosition::Right:
		Device = bhaptics::PositionType::Right;
		break;
	case EPosition::Head:
		Device = bhaptics::PositionType::Head;
		break;
	case EPosition::HandL:
		Device = bhaptics::PositionType::HandL;
		break;
	case EPosition::HandR:
		Device = bhaptics::PositionType::HandR;
		break;
	case EPosition::FootL:
		Device = bhaptics::PositionType::FootL;
		break;
	case EPosition::FootR:
		Device = bhaptics::PositionType::FootR;
		break;
	case EPosition::ForearmL:
		Device = bhaptics::PositionType::ForearmL;
		break;
	case EPosition::ForearmR:
		Device = bhaptics::PositionType::ForearmR;
		break;
	case EPosition::VestFront:
		Device = bhaptics::PositionType::Vest;
		break;
	case EPosition::VestBack:
		Device = bhaptics::PositionType::Vest;
		break;
	default:
		//return false;
		break;
	}
	return Device;
}
#endif // PLATFORM_ANDROID

FString PositionEnumToString(EPosition Position)
{
	FString PositionString = "All";

	switch (Position)
	{
	case EPosition::Left:
		PositionString = "Left";
		break;
	case EPosition::Right:
		PositionString = "Right";
		break;
	case EPosition::Head:
		PositionString = "Head";
		break;
	case EPosition::HandL:
		PositionString = "HandL";
		break;
	case EPosition::HandR:
		PositionString = "HandR";
		break;
	case EPosition::FootL:
		PositionString = "FootL";
		break;
	case EPosition::FootR:
		PositionString = "FootR";
		break;
	case EPosition::ForearmL:
		PositionString = "ForearmL";
		break;
	case EPosition::ForearmR:
		PositionString = "ForearmR";
		break;
	case EPosition::VestFront:
		PositionString = "VestFront";
		break;
	case EPosition::VestBack:
		PositionString = "VestBack";
		break;
	default:
		break;
	}

	return PositionString;
}

