// Fill out your copyright notice in the Description page of Project Settings.

#include "HapticsManagerPrivatePCH.h"
#include "HapticsManager.h"
#include "HapticComponent.h"
#include "HapticStructures.h"
#include "BhapticsUtilities.h"

FCriticalSection UHapticComponent::m_Mutex;
FString UHapticComponent::HapticFileRootFolderStatic = "";

// Sets default values for this component's properties
UHapticComponent::UHapticComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TArray<FString> HapticFiles;
	if (bhaptics::HapticPlayer::instance()->anyFilesLoaded())
	{
		FString FileDirectory = LoadFeedbackFiles(HapticFiles);
		for (int i = 0; i < HapticFiles.Num(); i++)
		{
			FString FileName = HapticFiles[i];
			FString FilePath = FileDirectory;
			int32 index;
			FileName.FindChar(TCHAR('.'), index);
			FilePath.Append("/");
			FilePath.Append(FileName);
			FString Key = FileName.Left(index);
			HapticFileNames.AddUnique(*Key);//remove?
		}

		HapticFileRootFolder = FileDirectory;//remove?
	}
	else
	{
		//get file names?
		std::vector<std::string> keys = bhaptics::HapticPlayer::instance()->fileNames();
		for (int i = 0; i < keys.size(); i++)
		{
			HapticFileNames.AddUnique(keys[0].c_str());
		}
	}
}

// Called every frame
void UHapticComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (IsTicking)
	{
		return;
	}

	if (!IsInitialised)
	{
		return;
	}

	IsTicking = true;

	bhaptics::HapticPlayer::instance()->doRepeat();

	IsTicking = false;
}

void UHapticComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BhapticsUtilities::Initialise())
	{
		FString temp = BhapticsUtilities::GetExecutablePath();

		if (!BhapticsUtilities::IsPlayerRunning())
		{
			UE_LOG(LogTemp, Log, TEXT("Player is not running"));

			if (BhapticsUtilities::IsPlayerInstalled())
			{
				UE_LOG(LogTemp, Log, TEXT("Player is installed - launching"));
				BhapticsUtilities::LaunchPlayer();
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Player is not Installed"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Player is running"));
		}

		BhapticsUtilities::Free();
	}

	//bhaptics::HapticPlayer::instance()->init();

	TArray<FString> HapticFiles;
	if (bhaptics::HapticPlayer::instance()->anyFilesLoaded())
	{
		FString FileDirectory = LoadFeedbackFiles(HapticFiles);
		for (int i = 0; i < HapticFiles.Num(); i++)
		{
			FString FileName = HapticFiles[i];
			FString FilePath = FileDirectory;
			int32 index;
			FileName.FindChar(TCHAR('.'), index);
			FilePath.Append("/");
			FilePath.Append(FileName);
			FString Key = FileName.Left(index);
			RegisterFeeback(Key, FilePath);
			HapticFileNames.AddUnique(*Key);//remove?
		}

		HapticFileRootFolder = FileDirectory;//remove?
		HapticFileRootFolderStatic = FileDirectory;
	}
	else
	{
		//get file names?
		std::vector<std::string> keys = bhaptics::HapticPlayer::instance()->fileNames();
		for (int i = 0; i < keys.size(); i++)
		{
			HapticFileNames.AddUnique(keys[0].c_str());
		}
		
		HapticFileRootFolder = HapticFileRootFolderStatic;
	}
	IsInitialised = true;
}

void UHapticComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//bhaptics::HapticPlayer::instance()->turnOff();
	//bhaptics::HapticPlayer::instance()->destroy();
	//bhaptics::HapticPlayer::instance() = nullptr;
}


void UHapticComponent::SubmitKey(UFeedbackFile* Feedback)
{
	if (!IsInitialised)
	{
		return;
	}

	std::string StandardKey(TCHAR_TO_UTF8(*Feedback->Key));
	if (!bhaptics::HapticPlayer::instance()->isFeedbackRegistered(StandardKey))
	{
		TSharedPtr<FJsonObject> JsonProject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Feedback->ProjectString);

		if (FJsonSerializer::Deserialize(Reader, JsonProject))
		{
			bhaptics::HapticPlayer::instance()->registerFeedback(StandardKey, JsonProject);
		}
	}
	bhaptics::HapticPlayer::instance()->submitRegistered(StandardKey);
}

void UHapticComponent::SubmitKeyWithIntensityDuration(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption, FScaleOption ScaleOption)
{
	if (!IsInitialised)
	{
		return;
	}
	
	std::string StandardKey(TCHAR_TO_UTF8(*Feedback->Key));
	std::string StandardAltKey(TCHAR_TO_UTF8(*AltKey));
	bhaptics::RotationOption RotateOption;
	bhaptics::ScaleOption Option;
	RotateOption.OffsetAngleX = RotationOption.OffsetAngleX;
	RotateOption.OffsetY = RotationOption.OffsetY;

	Option.Intensity = ScaleOption.Intensity;
	Option.Duration = ScaleOption.Duration;
	
	if (!bhaptics::HapticPlayer::instance()->isFeedbackRegistered(StandardKey))
	{
		TSharedPtr<FJsonObject> JsonProject = MakeShareable(new FJsonObject(Feedback->Project));
		bhaptics::HapticPlayer::instance()->registerFeedback(StandardKey, JsonProject);
	}

	bhaptics::HapticPlayer::instance()->submitRegistered(StandardKey, StandardAltKey, Option, RotateOption);
}

void UHapticComponent::SubmitKeyWithTransform(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption)
{
	if (!IsInitialised)
	{
		return;
	}
	SubmitKeyWithIntensityDuration(Feedback, AltKey, RotationOption, FScaleOption(1, 1));
}

void UHapticComponent::RegisterFeeback(const FString &Key, const FString &FilePath)
{
	std::string stdKey(TCHAR_TO_UTF8(*Key));

	FString newPath = FilePath;
	std::string StandardPath(TCHAR_TO_UTF8(*newPath));

	if (!FPaths::FileExists(newPath))
	{
		UE_LOG(LogTemp, Log, TEXT("File does not exist : %s"), *newPath);
		return;
	}

	//bhaptics::HapticPlayer::instance()->registerFeedback(stdKey, StandardPath);
}

FString UHapticComponent::LoadFeedbackFiles(TArray<FString>& FilesOut)//not needed
{
	FString RootFolderFullPath = FPaths::GameContentDir() + ProjectFeedbackFolder;
	if (!FPaths::DirectoryExists(RootFolderFullPath) || !UseProjectFeedbackFolder)
	{
		RootFolderFullPath = IPluginManager::Get().FindPlugin("HapticsManager")->GetBaseDir() + "/Feedback";
	}
	UE_LOG(LogTemp, Log, TEXT("Looking in dir : %s"), *RootFolderFullPath);

	FPaths::NormalizeDirectoryName(RootFolderFullPath);
	IFileManager& FileManager = IFileManager::Get();

	FString Ext = "*.tactosy";
	FString Ext2 = "*.tact";

	FString FinalPath = RootFolderFullPath + "/" + Ext;
	FString FinalPath2 = RootFolderFullPath + "/" + Ext2;
	FileManager.FindFiles(FilesOut, *FinalPath, true, false);
	FileManager.FindFiles(FilesOut, *FinalPath2, true, false);
	return RootFolderFullPath;
}

void UHapticComponent::SubmitBytes(const FString &Key, EPosition PositionEnum, const TArray<uint8>& InputBytes, int32 DurationInMilliSecs)
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::Position HapticPosition = bhaptics::Position::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));

	switch (PositionEnum)
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

	if (InputBytes.Num() != 20)
	{
		printf("Invalid Point Array\n");
		return;
	}

	std::vector<uint8_t> SubmittedDots(20, 0);

	for (int32 i = 0; i < InputBytes.Num(); i++)
	{
		SubmittedDots[i] = InputBytes[i];
	}

	bhaptics::HapticPlayer::instance()->submit(StandardKey, HapticPosition, SubmittedDots, DurationInMilliSecs);
}

void UHapticComponent::SubmitDots(const FString &Key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs)
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::Position HapticPosition = bhaptics::Position::All;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	switch (PositionEnum)
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

	for (int32 i = 0; i < DotPoints.Num(); i++)
	{
		SubmittedDots.push_back(bhaptics::DotPoint(DotPoints[i].Index, DotPoints[i].Intensity));
	}

	bhaptics::HapticPlayer::instance()->submit(StandardKey, HapticPosition, SubmittedDots, DurationInMilliSecs);
}

void UHapticComponent::SubmitPath(const FString &Key, EPosition PositionEnum, const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs)
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::Position HapticPosition = bhaptics::Position::All;
	std::vector<bhaptics::PathPoint> PathVector;
	std::string StandardKey(TCHAR_TO_UTF8(*Key));

	for (int32 i = 0; i < PathPoints.Num(); i++)
	{
		bhaptics::PathPoint Point(PathPoints[i].X, PathPoints[i].Y, PathPoints[i].Intensity, PathPoints[i].MotorCount);
		PathVector.push_back(Point);
	}

	switch (PositionEnum)
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

	bhaptics::HapticPlayer::instance()->submit(StandardKey, HapticPosition, PathVector, DurationInMilliSecs);
}

bool UHapticComponent::IsAnythingPlaying()
{
	return bhaptics::HapticPlayer::instance()->isPlaying();
}

bool UHapticComponent::IsRegisteredPlaying(const FString &Key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	return bhaptics::HapticPlayer::instance()->isPlaying(StandardKey); //can use gui?
}

void UHapticComponent::TurnOffAllFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->turnOff();
}

void UHapticComponent::TurnOffRegisteredFeedback(const FString &Key)
{
	if (!IsInitialised)
	{
		return;
	}

	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	bhaptics::HapticPlayer::instance()->turnOff(StandardKey);
}

void UHapticComponent::Reset()
{
	FHapticFeedback BlankFeedback = FHapticFeedback();

	bhaptics::HapticPlayer::instance()->destroy();
	bhaptics::HapticPlayer::instance()->init();
}

void UHapticComponent::EnableFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->enableFeedback();
}

void UHapticComponent::DisableFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->disableFeedback();
}

void UHapticComponent::ToggleFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->toggleFeedback();
}

bool UHapticComponent::IsDeviceConnected(EPosition device)
{
	bhaptics::Position pos = bhaptics::Position::All;

	switch (device)
	{
	case EPosition::Left:
		pos = bhaptics::Position::Left;
		break;
	case EPosition::Right:
		pos = bhaptics::Position::Right;
		break;
	case EPosition::Head:
		pos = bhaptics::Position::Head;
		break;
	case EPosition::Racket:
		pos = bhaptics::Position::Racket;
		break;
	case EPosition::HandL:
		pos = bhaptics::Position::HandL;
		break;
	case EPosition::HandR:
		pos = bhaptics::Position::HandR;
		break;
	case EPosition::FootL:
		pos = bhaptics::Position::FootL;
		break;
	case EPosition::FootR:
		pos = bhaptics::Position::FootR;
		break;
	case EPosition::VestFront:
		pos = bhaptics::Position::Vest;
		break;
	case EPosition::VestBack:
		pos = bhaptics::Position::Vest;
		break;
	case EPosition::Vest:
		pos = bhaptics::Position::Vest;
		break;
	default:
		return false;
		break;
	}

	return bhaptics::HapticPlayer::instance()->isDevicePlaying(pos);
}

