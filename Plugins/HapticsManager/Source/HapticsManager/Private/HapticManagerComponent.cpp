// Fill out your copyright notice in the Description page of Project Settings.

#include "HapticsManagerPrivatePCH.h"
#include "HapticsManager.h"
#include "HapticManagerComponent.h"
#include "HapticStructures.h"
#include "BhapticsUtilities.h"

FCriticalSection UHapticManagerComponent::m_Mutex;
FString UHapticManagerComponent::HapticFileRootFolderStatic = "";

// Sets default values for this component's properties
UHapticManagerComponent::UHapticManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	FGuid Gui = FGuid::NewGuid();
	Id = Gui.ToString();
}

// Called every frame
void UHapticManagerComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UHapticManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	std::string StandardId(TCHAR_TO_UTF8(*Id));
	UE_LOG(LogTemp, Log, TEXT("Initialise %s."), StandardId.c_str());
	bhaptics::HapticPlayer::instance()->registerConnection(StandardId);

	IsInitialised = true;
}

void UHapticManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	std::string StandardId(TCHAR_TO_UTF8(*Id));
	bhaptics::HapticPlayer::instance()->unregisterConnection(StandardId);
	
}


void UHapticManagerComponent::SubmitFeedback(UFeedbackFile* Feedback)
{
	if (!IsInitialised || Feedback == NULL)
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

void UHapticManagerComponent::SubmitFeedbackWithIntensityDuration(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption, FScaleOption ScaleOption)
{
	if (!IsInitialised || Feedback == NULL)
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
		TSharedPtr<FJsonObject> JsonProject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Feedback->ProjectString);

		if (FJsonSerializer::Deserialize(Reader, JsonProject))
		{
			bhaptics::HapticPlayer::instance()->registerFeedback(StandardKey, JsonProject);
		}
	}

	bhaptics::HapticPlayer::instance()->submitRegistered(StandardKey, StandardAltKey, Option, RotateOption);
}

void UHapticManagerComponent::SubmitFeedbackWithTransform(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption)
{
	if (!IsInitialised || Feedback == NULL)
	{
		return;
	}
	SubmitFeedbackWithIntensityDuration(Feedback, AltKey, RotationOption, FScaleOption(1, 1));
}

void UHapticManagerComponent::RegisterFeedbackFile(const FString &Key, UFeedbackFile* Feedback )
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));

	if (!bhaptics::HapticPlayer::instance()->isFeedbackRegistered(StandardKey))
	{
		TSharedPtr<FJsonObject> JsonProject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Feedback->ProjectString);

		if (FJsonSerializer::Deserialize(Reader, JsonProject))
		{
			bhaptics::HapticPlayer::instance()->registerFeedback(StandardKey, JsonProject);
		}
	}
}

void UHapticManagerComponent::SubmitBytes(const FString &Key, EPosition PositionEnum, const TArray<uint8>& InputBytes, int32 DurationInMilliSecs)
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

void UHapticManagerComponent::SubmitDots(const FString &Key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs)
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

void UHapticManagerComponent::SubmitPath(const FString &Key, EPosition PositionEnum, const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs)
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

bool UHapticManagerComponent::IsAnythingPlaying()
{
	return bhaptics::HapticPlayer::instance()->isPlaying();
}

bool UHapticManagerComponent::IsRegisteredPlaying(const FString &Key)
{
	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	return bhaptics::HapticPlayer::instance()->isPlaying(StandardKey); //can use gui?
}

void UHapticManagerComponent::TurnOffAllFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->turnOff();
}

void UHapticManagerComponent::TurnOffRegisteredFeedback(const FString &Key)
{
	if (!IsInitialised)
	{
		return;
	}

	std::string StandardKey(TCHAR_TO_UTF8(*Key));
	bhaptics::HapticPlayer::instance()->turnOff(StandardKey);
}

void UHapticManagerComponent::Reset()
{
	bhaptics::HapticPlayer::instance()->destroy();
	bhaptics::HapticPlayer::instance()->init();
}

void UHapticManagerComponent::EnableFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->enableFeedback();
}

void UHapticManagerComponent::DisableFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->disableFeedback();
}

void UHapticManagerComponent::ToggleFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	bhaptics::HapticPlayer::instance()->toggleFeedback();
}

FRotationOption UHapticManagerComponent::ProjectToVest(FVector Location, UPrimitiveComponent * HitComponent, float HalfHeight)
{
	FRotator InverseRotation = HitComponent->GetComponentRotation().GetInverse();
	FVector HitPoint = InverseRotation.RotateVector(Location - HitComponent->GetComponentLocation());
	FVector UpVector = InverseRotation.RotateVector(HitComponent->GetUpVector());
	FVector ForwardVector = InverseRotation.RotateVector(HitComponent->GetForwardVector());
	FVector Scale = HitComponent->GetComponentScale();
	float DotProduct, Angle, Y_Offset, A, B;
	FVector Result;

	UpVector.Normalize();
	ForwardVector.Normalize();

	HitPoint.X = HitPoint.X / Scale.X;
	HitPoint.Y = HitPoint.Y / Scale.Y;
	HitPoint.Z = HitPoint.Z / Scale.Z;

	DotProduct = FVector::DotProduct(HitPoint,UpVector);
	
	Result = HitPoint - (DotProduct * UpVector);
	Result.Normalize();

	A = ForwardVector.X * Result.Y - Result.X * ForwardVector.Y;
	B = ForwardVector.X * Result.X + Result.Y * ForwardVector.Y;

	Angle = FMath::RadiansToDegrees(FMath::Atan2(A, B));

	Y_Offset = FMath::Clamp(DotProduct / (HalfHeight * 2), -0.5f, 0.5f);

	return FRotationOption(Angle,Y_Offset);
}

bool UHapticManagerComponent::IsDeviceConnected(EPosition device)
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

