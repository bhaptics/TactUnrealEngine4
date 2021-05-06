//Copyright bHaptics Inc. 2017-2019

#include "BhapticsLibrary.h"
#include "BhapticsUtils.h"
#include "Interfaces/IPluginManager.h"

#include "Core/Public/Misc/ConfigCacheIni.h"
#include "Core/Public/Misc/Paths.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include <string>

#if PLATFORM_ANDROID
#include "AndroidHapticLibrary.h"
#else
#include "HapticLibrary/HapticLibrary.h"
#endif // PLATFORM_ANDROID


bool BhapticsLibrary::IsInitialised = false;
bool BhapticsLibrary::IsLoaded = false;
FProcHandle BhapticsLibrary::Handle;
bool BhapticsLibrary::Success = false;

#if PLATFORM_ANDROID
#else
static bhaptics::PositionType PositionEnumToDeviceType(EPosition Position);
static bhaptics::PositionType PositionEnumToPositionType(EPosition Position);
#endif

FString ProjectName = "AppName";
std::string projectName;


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


bool BhapticsLibrary::Initialize()
{
	if (GConfig) {
		FString fromString = " ";
		FString toString = "";
		const TCHAR* from = *fromString;
		const TCHAR* to = *toString;
		GConfig->GetString(
			TEXT("/Script/EngineSettings.GeneralProjectSettings"),
			TEXT("ProjectName"),
			ProjectName,
			GGameIni
		);
		ProjectName = ProjectName.Replace(from, to);
		projectName = (TCHAR_TO_UTF8(*ProjectName));
		UE_LOG(LogTemp, Log, TEXT("BhapticsLibrary Constructor: %s."), *ProjectName);
	}
#if PLATFORM_ANDROID
	IsInitialised = UAndroidHapticLibrary::AndroidThunkCpp_Initialize(ProjectName);
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
		char Path[1000];
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
	Initialise(projectName.c_str(), projectName.c_str());
	Success = true;
#endif 
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
	InitializeCheck();
	
#if PLATFORM_ANDROID
	UAndroidHapticLibrary::RegisterProject(Key, ProjectJson);
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

void BhapticsLibrary::Lib_RegisterReflectedFeedback(FString Key, FString ProjectJson)
{
	InitializeCheck();

#if PLATFORM_ANDROID
	UAndroidHapticLibrary::RegisterProjectReflected(Key, ProjectJson);
#else
	if (!IsLoaded)
	{
		return;
	}

	FString s = "{\"project\":" + ProjectJson + ", \"intervalMillis\": 20, \"size\": 20, \"durationMillis\": 1000}";

	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	std::string ProjectString =  (TCHAR_TO_UTF8(*s));
	RegisterFeedbackFromTactFileReflected(StandardKey.c_str(), ProjectString.c_str());
#endif
}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key)
{
	InitializeCheck();
	
#if PLATFORM_ANDROID
	UAndroidHapticLibrary::SubmitRegistered(Key, Key, 1, 1, 0, 0);
#else
	if (!IsLoaded)
	{
		return;
	}
	const std::string stdKey(TCHAR_TO_UTF8(*Key));
	SubmitRegistered(stdKey.c_str());
#endif // PLATFORM_ANDROID

}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption)
{

	InitializeCheck();
	
#if PLATFORM_ANDROID
	UAndroidHapticLibrary::SubmitRegistered(
		Key, AltKey, ScaleOpt.Intensity, ScaleOpt.Duration, RotOption.OffsetAngleX, RotOption.OffsetY);
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
	InitializeCheck();
	
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

	InitializeCheck();
	
#if PLATFORM_ANDROID
	UAndroidHapticLibrary::SubmitDot(Key, BhapticsUtils::PositionEnumToString(Pos), Points, DurationMillis);
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
	InitializeCheck();
#if PLATFORM_ANDROID
	UAndroidHapticLibrary::SubmitPath(Key, BhapticsUtils::PositionEnumToString(Pos), Points, DurationMillis);
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
		int XVal = Points[i].X * 1000;
		int YVal = Points[i].Y * 1000;
		bhaptics::PathPoint Point(XVal, YVal, Points[i].Intensity, Points[i].MotorCount);
		PathVector.push_back(Point);
	}

	SubmitPath(StandardKey.c_str(), HapticPosition, PathVector, DurationMillis);
#endif
}

bool BhapticsLibrary::Lib_IsFeedbackRegistered(FString Key)
{
	InitializeCheck();
	bool Value = false;
#if PLATFORM_ANDROID
	Value = UAndroidHapticLibrary::IsFeedbackRegistered(Key);
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
	InitializeCheck();
	
	bool Value = false;
#if PLATFORM_ANDROID
	return UAndroidHapticLibrary::IsAnyFeedbackPlaying();
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
	InitializeCheck();
	
	bool Value = false;
#if PLATFORM_ANDROID
	return UAndroidHapticLibrary::IsFeedbackPlaying(Key);
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
	InitializeCheck();
#if PLATFORM_ANDROID
	UAndroidHapticLibrary::TurnOffAll();
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
	InitializeCheck();
#if PLATFORM_ANDROID
	UAndroidHapticLibrary::TurnOff(Key);
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
	InitializeCheck();
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
	InitializeCheck();
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
	InitializeCheck();
#if PLATFORM_ANDROID
#else
	if (!IsLoaded)
	{
		return;
	}
	ToggleFeedback();
#endif
}

bool BhapticsLibrary::Lib_IsDeviceConnected(EPosition Pos)
{
	InitializeCheck();
	bool Value = false;
#if PLATFORM_ANDROID
	return UAndroidHapticLibrary::IsDeviceConnceted(Pos);
#else
	if (!IsLoaded)
	{
		return false;
	}
	bhaptics::PositionType device = PositionEnumToDeviceType(Pos);
	Value = IsDevicePlaying(device);
#endif
	return Value;
}

TArray<FHapticFeedback> BhapticsLibrary::Lib_GetResponseStatus()
{
	InitializeCheck();
	TArray<FHapticFeedback> ChangedFeedback;
#if PLATFORM_ANDROID
	FPlayerResponse Response;
	ChangedFeedback = Response.Status;
	TArray<EPosition> PositionEnum =
	{ EPosition::ForearmL,EPosition::ForearmR,EPosition::Head, EPosition::VestFront,EPosition::VestBack,EPosition::HandL, EPosition::HandR, EPosition::FootL, EPosition::FootR };

	for (int posIndex = 0; posIndex < PositionEnum.Num(); posIndex++)
	{
		EPosition pos = PositionEnum[posIndex];
		FString posStr = BhapticsUtils::PositionEnumToString(pos);
		TArray<uint8> val = UAndroidHapticLibrary::GetPositionStatus(posStr);

		FHapticFeedback Feedback = FHapticFeedback(pos, val, EFeedbackMode::DOT_MODE);
		ChangedFeedback.Add(Feedback);
	}
#else
	if (!IsLoaded)
	{
		return ChangedFeedback;
	}
	TArray<bhaptics::PositionType> Positions =
	{ bhaptics::PositionType::ForearmL,bhaptics::PositionType::ForearmR,bhaptics::PositionType::Head, bhaptics::PositionType::VestFront,bhaptics::PositionType::VestBack,
	bhaptics::PositionType::HandL, bhaptics::PositionType::HandR, bhaptics::PositionType::FootL, bhaptics::PositionType::FootR };
	TArray<EPosition> PositionEnum =
	{ EPosition::ForearmL,EPosition::ForearmR,EPosition::Head, EPosition::VestFront,EPosition::VestBack,EPosition::HandL, EPosition::HandR, EPosition::FootL, EPosition::FootR };

	for (int posIndex = 0; posIndex < Positions.Num(); posIndex++)
	{
		status stat;
		bool res = TryGetResponseForPosition(Positions[posIndex], stat);
		TArray<uint8> val;
		val.Init(0, 20);
		if (res)
		{
			for (int motorIndex = 0; motorIndex < val.Num(); motorIndex++)
			{
				val[motorIndex] = stat.values[motorIndex];
			}
		}

		FHapticFeedback Feedback = FHapticFeedback(PositionEnum[posIndex], val, EFeedbackMode::DOT_MODE);
		ChangedFeedback.Add(Feedback);
	}
#endif
	return ChangedFeedback;
}

TArray<uint8> BhapticsLibrary::Lib_GetResponseStatus(EPosition Pos)
{
	InitializeCheck();
#if PLATFORM_ANDROID

	FString posStr = BhapticsUtils::PositionEnumToString(Pos);
	TArray<uint8> val = UAndroidHapticLibrary::GetPositionStatus(posStr);
	return val;
#else
	TArray<uint8> val;
	val.Init(0, 20);
	if (!IsLoaded)
	{
		return val;
	}
	status stat;
	bool res = TryGetResponseForPosition(PositionEnumToPositionType(Pos), stat);
	if (res)
	{
		for (int motorIndex = 0; motorIndex < val.Num(); motorIndex++)
		{
			val[motorIndex] = stat.values[motorIndex];
		}

		return val;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Failed to get status"));

		return val;
	}
#endif
}

void BhapticsLibrary::InitializeCheck()
{
	if (!IsInitialised)
	{
		Initialize();
	}
}

#if !PLATFORM_ANDROID


static bhaptics::PositionType PositionEnumToDeviceType(EPosition Position)
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
		Device = bhaptics::PositionType::VestFront;
		break;
	case EPosition::VestBack:
		Device = bhaptics::PositionType::VestBack;
		break;
	default:
		//return false;
		break;
	}
	return Device;
}
#endif // !PLATFORM_ANDROID