//Copyright bHaptics Inc. 2017-2019


#include "BhapticsRequestLibrary.h"


#include "HapticStructures.h"
#include "Components/PrimitiveComponent.h"
#include "BhapticsLibrary.h"

void UBhapticsRequestLibrary::SubmitFeedback(UFeedbackFile* Feedback)
{
	if (Feedback == NULL)
	{
		return;
	}

	FString FeedbackKey = Feedback->Key + Feedback->Id.ToString();

	if (!BhapticsLibrary::Lib_IsFeedbackRegistered(FeedbackKey))
	{
		BhapticsLibrary::Lib_RegisterFeedback(FeedbackKey, Feedback->ProjectString);
	}
	BhapticsLibrary::Lib_SubmitRegistered(FeedbackKey);
}

void UBhapticsRequestLibrary::SubmitFeedbackWithIntensityDuration(UFeedbackFile* Feedback, const FString& AltKey, FRotationOption RotationOption, FScaleOption ScaleOption, bool UseAltKey)
{
	if (Feedback == NULL)
	{
		return;
	}

	FString FeedbackKey = Feedback->Key + Feedback->Id.ToString();
	FString UniqueKey = AltKey;

	if (!UseAltKey)
	{
		UniqueKey = Feedback->Key + FString::FromInt(FMath::Rand());
	}

	if (!BhapticsLibrary::Lib_IsFeedbackRegistered(FeedbackKey))
	{
		BhapticsLibrary::Lib_RegisterFeedback(FeedbackKey, Feedback->ProjectString);
	}

	BhapticsLibrary::Lib_SubmitRegistered(FeedbackKey, UniqueKey, ScaleOption, RotationOption);
}

void UBhapticsRequestLibrary::SubmitFeedbackWithTransform(UFeedbackFile* Feedback, const FString& AltKey, FRotationOption RotationOption, bool UseAltKey)
{
	if (Feedback == NULL)
	{
		return;
	}
	SubmitFeedbackWithIntensityDuration(Feedback, AltKey, RotationOption, FScaleOption(1, 1), UseAltKey);
}

void UBhapticsRequestLibrary::RegisterFeedbackFile(const FString& Key, UFeedbackFile* Feedback)
{
	if (Feedback == NULL)
	{
		return;
	}
	if (!BhapticsLibrary::Lib_IsFeedbackRegistered(Key))
	{
		BhapticsLibrary::Lib_RegisterFeedback(Key, Feedback->ProjectString);
	}
}

void UBhapticsRequestLibrary::SubmitBytes(const FString& Key, EPosition PositionEnum, const TArray<uint8>& InputBytes, int32 DurationInMilliSecs)
{
	BhapticsLibrary::Lib_Submit(Key, PositionEnum, InputBytes, DurationInMilliSecs);
}

void UBhapticsRequestLibrary::SubmitDots(const FString& Key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs)
{
	BhapticsLibrary::Lib_Submit(Key, PositionEnum, DotPoints, DurationInMilliSecs);
}

void UBhapticsRequestLibrary::SubmitPath(const FString& Key, EPosition PositionEnum, const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs)
{
	BhapticsLibrary::Lib_Submit(Key, PositionEnum, PathPoints, DurationInMilliSecs);
}

bool UBhapticsRequestLibrary::IsAnythingPlaying()
{
	bool Value = false;
	Value = BhapticsLibrary::Lib_IsPlaying();
	return Value;
}

bool UBhapticsRequestLibrary::IsRegisteredPlaying(const FString& Key)
{
	return  BhapticsLibrary::Lib_IsPlaying(Key);
}

bool UBhapticsRequestLibrary::IsRegisteredFilePlaying(UFeedbackFile* Feedback)
{
	bool Value = false;
	if (Feedback == NULL)
	{
		return Value;
	}
	FString FeedbackKey = Feedback->Key + Feedback->Id.ToString();

	Value = BhapticsLibrary::Lib_IsPlaying(FeedbackKey);
	return Value;
}

void UBhapticsRequestLibrary::TurnOffAllFeedback()
{
	BhapticsLibrary::Lib_TurnOff();
}

void UBhapticsRequestLibrary::TurnOffRegisteredFeedback(const FString& Key)
{
	BhapticsLibrary::Lib_TurnOff(Key);
}

void UBhapticsRequestLibrary::TurnOffRegisteredFeedbackFile(UFeedbackFile* Feedback)
{
	if (Feedback == NULL)
	{
		return;
	}
	FString FeedbackKey = Feedback->Key + Feedback->Id.ToString();

	BhapticsLibrary::Lib_TurnOff(FeedbackKey);
}

void UBhapticsRequestLibrary::EnableHapticFeedback()
{
	BhapticsLibrary::Lib_EnableFeedback();
}

void UBhapticsRequestLibrary::DisableHapticFeedback()
{
	BhapticsLibrary::Lib_DisableFeedback();
}

void UBhapticsRequestLibrary::ToggleHapticFeedback()
{
	BhapticsLibrary::Lib_ToggleFeedback();
}

FRotationOption UBhapticsRequestLibrary::ProjectToVest(FVector Location, UPrimitiveComponent* HitComponent, float HalfHeight)
{
	if (HitComponent == nullptr)
	{
		return FRotationOption(0, 0);
	}

	FRotator InverseRotation = HitComponent->GetComponentRotation().GetInverse();
	FVector HitPoint = InverseRotation.RotateVector(Location - HitComponent->GetComponentLocation());
	FVector UpVector = FVector::UpVector;//InverseRotation.RotateVector(HitComponent->GetUpVector());
	FVector ForwardVector = FVector::ForwardVector;//InverseRotation.RotateVector(HitComponent->GetForwardVector());
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

FRotationOption UBhapticsRequestLibrary::ProjectToVestLocation(FVector ContactLocation, FVector PlayerLocation,
	FRotator PlayerRotation)
{
	FRotator InverseRotation = PlayerRotation.GetInverse();
	FVector HitPoint = InverseRotation.RotateVector(ContactLocation - PlayerLocation);
	FVector UpVector = FVector::UpVector;//InverseRotation.RotateVector(HitComponent->GetUpVector());
	FVector ForwardVector = FVector::ForwardVector;//InverseRotation.RotateVector(HitComponent->GetForwardVector());
	float DotProduct, Angle, A, B;
	FVector Result;

	UpVector.Normalize();
	ForwardVector.Normalize();

	DotProduct = FVector::DotProduct(HitPoint, UpVector);

	Result = HitPoint - (DotProduct * UpVector);
	Result.Normalize();

	A = Result.X * ForwardVector.Y - ForwardVector.X * Result.Y;
	B = ForwardVector.X * Result.X + Result.Y * ForwardVector.Y;

	Angle = FMath::RadiansToDegrees(FMath::Atan2(A, B));

	return FRotationOption(Angle, 0);
}


FRotationOption UBhapticsRequestLibrary::CustomProjectToVest(FVector Location, UPrimitiveComponent* HitComponent, float HalfHeight, FVector UpVector, FVector ForwardVector)
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

float UBhapticsRequestLibrary::GetAngle(FVector ContactLocation, FVector PlayerLocation, FVector PlayerForward)
{
	FVector targetDir = ContactLocation - PlayerLocation;
	targetDir.Normalize();
	
	FVector2D playerForwardVector = FVector2D(PlayerForward.X, PlayerForward.Y);
	FVector2D pointDirection = FVector2D(targetDir.X, targetDir.Y);

	playerForwardVector.Normalize();
	pointDirection.Normalize();

	float v1 = FVector2D::DotProduct(playerForwardVector, pointDirection);

	float v2 = FMath::Sqrt(FMath::Pow(pointDirection.X, 2)+ FMath::Pow(pointDirection.Y, 2));
	float v3 = FMath::Sqrt(FMath::Pow(playerForwardVector.X, 2)+ FMath::Pow(playerForwardVector.Y, 2));

	float v = v1 / (v2 * v3);
	float angle = FMath::RadiansToDegrees(FMath::Acos(v));

	// if left for right
	float a = playerForwardVector.X * pointDirection.Y - playerForwardVector.Y * pointDirection.X;
	if (a > 0) // 180 ~ 360 
	{
		
		angle = 360 - angle;
	}

	

	return angle;
}

float UBhapticsRequestLibrary::GetAngleHitNormal(FVector HitNormal, FVector PlayerForward)
{
	FVector targetDir = HitNormal;
	targetDir.Normalize();

	FVector2D playerForwardVector = FVector2D(PlayerForward.X, PlayerForward.Y);
	FVector2D pointDirection = FVector2D(targetDir.X, targetDir.Y);

	playerForwardVector.Normalize();
	pointDirection.Normalize();

	float v1 = FVector2D::DotProduct(playerForwardVector, pointDirection);

	float v2 = FMath::Sqrt(FMath::Pow(pointDirection.X, 2) + FMath::Pow(pointDirection.Y, 2));
	float v3 = FMath::Sqrt(FMath::Pow(playerForwardVector.X, 2) + FMath::Pow(playerForwardVector.Y, 2));

	float v = v1 / (v2 * v3);
	float angle = FMath::RadiansToDegrees(FMath::Acos(v));

	// if left for right
	float a = playerForwardVector.X * pointDirection.Y - playerForwardVector.Y * pointDirection.X;
	if (a > 0) // 180 ~ 360 
	{

		angle = 360 - angle;
	}



	return angle;
}


bool UBhapticsRequestLibrary::IsDeviceConnected(EPosition device)
{
	return BhapticsLibrary::Lib_IsDevicePlaying(device);
}
