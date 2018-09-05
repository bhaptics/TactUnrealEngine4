//Copyright bHaptics Inc. 2018

#if (ENGINE_MINOR_VERSION < 16)
//#include "HapticsManager.h"
#endif
#include "HapticManagerComponent.h"
#include "HapticStructures.h"
#include "BhapticsUtilities.h"

#include "BhapticsLibrary.h"

FCriticalSection UHapticManagerComponent::m_Mutex;

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
void UHapticManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHapticManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	IsInitialised = true;
}


void UHapticManagerComponent::SubmitFeedback(UFeedbackFile* Feedback)
{
	if (!IsInitialised || Feedback == NULL)
	{
		return;
	}

	if (!BhapticsLibrary::Lib_IsFeedbackRegistered(Feedback->Key))
	{
		BhapticsLibrary::Lib_RegisterFeedback(Feedback->Key, Feedback->ProjectString);
	}
	BhapticsLibrary::Lib_SubmitRegistered(Feedback->Key);
}

void UHapticManagerComponent::SubmitFeedbackWithIntensityDuration(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption, FScaleOption ScaleOption)
{
	if (!IsInitialised || Feedback == NULL)
	{
		return;
	}

	if (!BhapticsLibrary::Lib_IsFeedbackRegistered(Feedback->Key))
	{
		BhapticsLibrary::Lib_RegisterFeedback(Feedback->Key, Feedback->ProjectString);
	}

	BhapticsLibrary::Lib_SubmitRegistered(Feedback->Key, AltKey, ScaleOption, RotationOption);
}

void UHapticManagerComponent::SubmitFeedbackWithTransform(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption)
{
	if (!IsInitialised || Feedback == NULL)
	{
		return;
	}
	SubmitFeedbackWithIntensityDuration(Feedback, AltKey, RotationOption, FScaleOption(1, 1));
}

void UHapticManagerComponent::RegisterFeedbackFile(const FString &Key, UFeedbackFile* Feedback)
{
	if (!BhapticsLibrary::Lib_IsFeedbackRegistered(Key))
	{
		BhapticsLibrary::Lib_RegisterFeedback(Key, Feedback->ProjectString);
	}
}

void UHapticManagerComponent::SubmitBytes(const FString &Key, EPosition PositionEnum, const TArray<uint8>& InputBytes, int32 DurationInMilliSecs)
{
	if (!IsInitialised)
	{
		return;
	}

	BhapticsLibrary::Lib_Submit(Key, PositionEnum, InputBytes, DurationInMilliSecs);
}

void UHapticManagerComponent::SubmitDots(const FString &Key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs)
{
	if (!IsInitialised)
	{
		return;
	}
	BhapticsLibrary::Lib_Submit(Key, PositionEnum, DotPoints, DurationInMilliSecs);
}

void UHapticManagerComponent::SubmitPath(const FString &Key, EPosition PositionEnum, const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs)
{
	if (!IsInitialised)
	{
		return;
	}
	BhapticsLibrary::Lib_Submit(Key, PositionEnum, PathPoints, DurationInMilliSecs);
}

bool UHapticManagerComponent::IsAnythingPlaying()
{
	bool Value = false;
	//return bhaptics::HapticPlayer::instance()->isPlaying();
	Value = BhapticsLibrary::Lib_IsPlaying();
	return Value;
}

bool UHapticManagerComponent::IsRegisteredPlaying(const FString &Key)
{
	bool Value = false;
	Value = BhapticsLibrary::Lib_IsPlaying(Key);
	return Value;
}

void UHapticManagerComponent::TurnOffAllFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	//bhaptics::HapticPlayer::instance()->turnOff();
	BhapticsLibrary::Lib_TurnOff();
}

void UHapticManagerComponent::TurnOffRegisteredFeedback(const FString &Key)
{
	if (!IsInitialised)
	{
		return;
	}
	//bhaptics::HapticPlayer::instance()->turnOff(StandardKey);
	BhapticsLibrary::Lib_TurnOff(Key);
}

void UHapticManagerComponent::EnableHapticFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	//bhaptics::HapticPlayer::instance()->enableFeedback();
	BhapticsLibrary::Lib_EnableFeedback();
}

void UHapticManagerComponent::DisableHapticFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	//bhaptics::HapticPlayer::instance()->disableFeedback();
	BhapticsLibrary::Lib_DisableFeedback();
}

void UHapticManagerComponent::ToggleHapticFeedback()
{
	if (!IsInitialised)
	{
		return;
	}
	//bhaptics::HapticPlayer::instance()->toggleFeedback();
	BhapticsLibrary::Lib_ToggleFeedback();
}

FRotationOption UHapticManagerComponent::ProjectToVest(FVector Location, UPrimitiveComponent * HitComponent, float HalfHeight)
{
	if (HitComponent == nullptr)
	{
		return FRotationOption(0, 0);
	}

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

	DotProduct = FVector::DotProduct(HitPoint, UpVector);

	Result = HitPoint - (DotProduct * UpVector);
	Result.Normalize();

	A = Result.X * ForwardVector.Y - ForwardVector.X * Result.Y;
	B = ForwardVector.X * Result.X + Result.Y * ForwardVector.Y;

	Angle = FMath::RadiansToDegrees(FMath::Atan2(A, B));

	Y_Offset = FMath::Clamp(DotProduct / (HalfHeight * 2), -0.5f, 0.5f);

	return FRotationOption(Angle, Y_Offset);
}

FRotationOption UHapticManagerComponent::CustomProjectToVest(FVector Location, UPrimitiveComponent * HitComponent, float HalfHeight, FVector UpVector, FVector ForwardVector)
{
	if (HitComponent == nullptr)
	{
		return FRotationOption(0, 0);
	}

	FRotator InverseRotation = HitComponent->GetComponentRotation().GetInverse();
	FVector HitPoint = InverseRotation.RotateVector(Location - HitComponent->GetComponentLocation());
	FVector Scale = HitComponent->GetComponentScale();
	float DotProduct, Angle, Y_Offset, A, B;
	FVector Result;

	if (UpVector == FVector::ZeroVector)
	{
		UpVector = InverseRotation.RotateVector(HitComponent->GetUpVector());
	}
	else
	{
		UpVector = InverseRotation.RotateVector(UpVector);
	}

	if (ForwardVector == FVector::ZeroVector)
	{
		ForwardVector = InverseRotation.RotateVector(HitComponent->GetForwardVector());
	}
	else
	{
		ForwardVector = InverseRotation.RotateVector(ForwardVector);
	}


	UpVector.Normalize();
	ForwardVector.Normalize();

	HitPoint.X = HitPoint.X / Scale.X;
	HitPoint.Y = HitPoint.Y / Scale.Y;
	HitPoint.Z = HitPoint.Z / Scale.Z;

	DotProduct = FVector::DotProduct(HitPoint, UpVector);

	Result = HitPoint - (DotProduct * UpVector);
	Result.Normalize();

	A = Result.X * ForwardVector.Y - ForwardVector.X * Result.Y + Result.Y * ForwardVector.Z - ForwardVector.Y * Result.Z + Result.Z * ForwardVector.X - ForwardVector.Z * Result.X;
	B = ForwardVector.X * Result.X + Result.Y * ForwardVector.Y + Result.Z * ForwardVector.Z;

	Angle = FMath::RadiansToDegrees(FMath::Atan2(A, B));

	Y_Offset = FMath::Clamp(DotProduct / (HalfHeight * 2), -0.5f, 0.5f);

	return FRotationOption(Angle, Y_Offset);
}

bool UHapticManagerComponent::IsDeviceConnected(EPosition device)
{

	bool Value = false;
	Value = BhapticsLibrary::Lib_IsDevicePlaying(device);
	return Value;
}

