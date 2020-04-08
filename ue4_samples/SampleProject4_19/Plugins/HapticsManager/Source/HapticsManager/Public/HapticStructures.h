//Copyright bHaptics Inc. 2017-2019

#pragma once
#include "Engine.h"
#include "HapticStructures.generated.h"

UENUM(BlueprintType)
enum class EPosition : uint8
{
	VestFront = 201, VestBack = 202,
	Head = 4,
	ForearmL = 10, ForearmR = 11,
	HandL = 6, HandR = 7,
	FootL = 8, FootR = 9,
	Left = 1, Right = 2,
	Default = 0 UMETA(Hidden),
};

UENUM(BlueprintType)
enum class EFeedbackMode : uint8
{
	PATH_MODE	UMETA(DisplayName = "PATHMODE"),
	DOT_MODE	UMETA(DisplayName = "DOTMODE")
};

//Structure used to play individual motors on each device.
USTRUCT(BlueprintType)
struct FDotPoint
{
	GENERATED_BODY()

		FDotPoint()
	{
		Index = 0;
		Intensity = 0;
	}

	FDotPoint(int32 _index, int32 _intensity)
	{
		Index = FMath::Clamp(_index, 0, 19);
		Intensity = FMath::Clamp(_intensity, 0, 100);
	}

	//Index of the motor to be activated.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		int32 Index;

	//Intensity of the vibration from 0 to 100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		int32 Intensity;
};

//Structure to allow for continuous haptic feedback anywhere on the device, interpolating which motors are played.
USTRUCT(BlueprintType)
struct FPathPoint
{
	GENERATED_BODY()
		FPathPoint()
	{
		X = 0;
		Y = 0;
		Intensity = 0;
		MotorCount = 3;
	}

	//X-value position on the device from 0.0 to 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		float X;

	//Y-value position on the device from 0.0 to 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		float Y;

	//Intensity of the vibration from 0 to 100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		int32 Intensity;

	//Number of motors activated when interpolating the point from 1 to 3.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		int32 MotorCount;

	FPathPoint(float _x, float _y, int32 _intensity, int32 _motorCount = 3)
	{
		int XRound = _x * 1000;
		int YRound = _y * 1000;
		X = XRound / 1000;
		Y = YRound / 1000;
		X = FMath::Clamp(X, 0.0f, 1.0f);
		Y = FMath::Clamp(Y, 0.0f, 1.0f);
		Intensity = FMath::Clamp(_intensity, 0, 100);
		MotorCount = FMath::Clamp(_motorCount, 1, 3);
	}
};

USTRUCT(BlueprintType)
struct FHapticFeedback
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		EPosition Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		EFeedbackMode Mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		TArray<uint8> Values;

	FHapticFeedback(EPosition _pos, const int _val[], EFeedbackMode _mod)
	{
		Position = _pos;
		Mode = _mod;
		Values.AddZeroed(20);
		for (int i = 0; i < 20; i++)
		{
			Values[i] = _val[i];
		}
	}

	FHapticFeedback(EPosition _pos, const TArray<uint8> &_val, EFeedbackMode _mod)
	{
		Position = _pos;
		Mode = _mod;
		Values = _val;
	}

	FHapticFeedback() : Position(), Mode()
	{
		Values.AddZeroed(20);
	}
};

//Stores values used in rotating and transforming a feedback file on the haptic vest.
USTRUCT(BlueprintType)
struct FRotationOption
{
	GENERATED_BODY()

	FRotationOption()
	{
		OffsetAngleX = 0;
		OffsetY = 0;
	}

	//Rotate the feedback file horizontally clockwise along the vest by the given angle in degrees.
	//A value of 180 will flip the feedback to the other side of the device.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		float OffsetAngleX = 0;

	//Vertical offset of the transformed feedback file, with negative values moving the feedback upwards.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		float OffsetY = 0;

	FRotationOption(float AngleX, float Y)
	{
		OffsetAngleX = AngleX;
		OffsetY = FMath::Clamp(Y,-1.0f,1.0f);
	}

	FString ToString()
	{
		return "{ \"offsetAngleX\" : " + FString::SanitizeFloat(OffsetAngleX) + ", \"offsetY\" : " + FString::SanitizeFloat(OffsetY) + "}";
	}
};

//Stores the scaled values when altering a feedback file's intensity and duration.
USTRUCT(BlueprintType)
struct FScaleOption
{
	GENERATED_BODY()

	FScaleOption()
	{
		Duration = 1.0;
 	}

	//Multiplier to scale the intensity of the feedback, where 0.5 halves the intensity
	//and a value of 2.0 doubles it. Cannot be negative.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		float Intensity = 1.0;

	//Multiplier to scale the duration of the feedback, where 0.5 halves the duration
	//and a value of 2.0 doubles it. Cannot be negative.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		float Duration = 1.0;

	FScaleOption(float intensity, float duration)
	{
		Intensity = FMath::Clamp(intensity,0.01f,100.0f);
		Duration = FMath::Clamp(duration, 0.01f, 100.0f);
	}

	FString ToString()
	{
		return "{ \"intensity\" : " + FString::SanitizeFloat(Intensity) + ", \"duration\" : " + FString::SanitizeFloat(Duration) + "}";
	}
};

USTRUCT(BlueprintType)
struct FDevice {

	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Android|Haptic")
		FString DeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Android|Haptic")
		FString Address;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Android|Haptic")
		FString Position;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Android|Haptic")
		int Battery;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Android|Haptic")
		FString ConnectionStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Android|Haptic")
		bool IsPaired;
};

USTRUCT()
struct FRegisterRequest {

	GENERATED_BODY()

	UPROPERTY()
	FString Key;

	//UPROPERTY()
	TSharedPtr<FJsonObject> Project;

	void ToJsonObject(FJsonObject& JsonObject)
	{
		JsonObject.SetStringField("Key", Key);
		JsonObject.SetObjectField("Project", Project);
	}
};

USTRUCT()
struct FHapticFrame {

	GENERATED_BODY()

	UPROPERTY()
	int DurationMillis;

	UPROPERTY()
		FString Position;

	UPROPERTY()
		TArray<FPathPoint> PathPoints;

	UPROPERTY()
		TArray<FDotPoint> DotPoints;
};


USTRUCT()
struct FSubmitRequest {

	GENERATED_BODY()

		FSubmitRequest()
	{
		Type = "turnoff";
		Key = "";
		Parameters = TMap<FString, FString>();
	}

	FSubmitRequest(FString SubmissionKey, FString SubmissionType, FHapticFrame SubmissionFrame)
	{
		Type = SubmissionType;
		Key = SubmissionKey;
		Frame = SubmissionFrame;
		Parameters = TMap<FString, FString>();
	}

	UPROPERTY()
		FString Type;

	UPROPERTY()
		FString Key;

	UPROPERTY()
		TMap<FString, FString> Parameters;

	UPROPERTY()
		FHapticFrame Frame;
};

USTRUCT()
struct FPlayerResponse {

	GENERATED_BODY()

		TArray<FString> RegisteredKeys;

	TArray<FString> ActiveKeys;

	int ConnectedDeviceCount;

	TArray<FString> ConnectedPositions;

	TArray<FHapticFeedback> Status;
};

USTRUCT()
struct FPlayerRequest {

	GENERATED_BODY()

		UPROPERTY()
		TArray<FRegisterRequest> Register;

	UPROPERTY()
		TArray<FSubmitRequest> Submit;
};

class HAPTICSMANAGER_API HapticStructures
{
public:
	HapticStructures();
	~HapticStructures();
};