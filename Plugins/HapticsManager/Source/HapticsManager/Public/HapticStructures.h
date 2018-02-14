//Copyright bHaptics Inc. 2017

#pragma once
#include "Engine.h"
#include "HapticStructures.generated.h"

UENUM(BlueprintType)
enum class EPosition : uint8
{
	All = 0, Left = 1, Right = 2,
	Vest = 3,
	Head = 4,
	Racket = 5,
	HandL = 6, HandR = 7,
	FootL = 8, FootR = 9,
	VestFront = 201, VestBack = 202,
	GloveLeft = 203, GloveRight = 204,
	Custom1 = 251, Custom2 = 252, Custom3 = 253, Custom4 = 254
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
struct FHapticFeedbackFrame
{
	GENERATED_BODY()

		FHapticFeedbackFrame()
	{
		Position = EPosition::All;
		PathPoints.Add(FPathPoint());
		DotPoints.Add(FDotPoint());
		Texture = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		TArray<FPathPoint> PathPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		TArray<FDotPoint> DotPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
		int32 Texture;

	UPROPERTY(BlueprintReadWrite, Category = Vars)
		EPosition Position;

	FHapticFeedbackFrame(EPosition _pos, const TArray<FPathPoint> &_pathPoints)
	{
		Position = _pos;
		PathPoints = _pathPoints;
		Texture = 0;
	}

	FHapticFeedbackFrame(EPosition _pos, const TArray<FDotPoint> &_dotPoints)
	{
		Position = _pos;
		DotPoints = _dotPoints;
		Texture = 0;
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
		//values.assign(20, 0);
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
};

//Stores the scaled values when altering a feedback file's intensity and duration.
USTRUCT(BlueprintType)
struct FScaleOption
{
	GENERATED_BODY()

	FScaleOption()
	{
		Duration = 1.0;
		Intensity = 1.0;
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
};

//UENUM()
//enum class EPlaybackType : uint8
//{
//	NONE,
//	FADE_IN,
//	FADE_OUT,
//	FADE_IN_OUT
//};
//
//UENUM()
//enum class EPathMovingPattern : uint8
//{
//	CONST_SPEED,
//	CONST_TDM
//};
//
//USTRUCT()
//struct FDotModeObject
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//	int index;
//
//	UPROPERTY()
//	float intensity;
//
//	void to_json(FJsonObject& j)
//	{
//		j.SetNumberField("Index", index);
//
//		j.SetStringField("Intensity", FString::SanitizeFloat(intensity));
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		index = j.GetNumberField("index");
//		intensity = j.GetNumberField("intensity");
//	}
//};
//
//USTRUCT()
//struct FDotModeObjectCollection
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//		int StartTime;
//
//	UPROPERTY()
//	int EndTime;
//
//	UPROPERTY()
//	EPlaybackType PlaybackType = EPlaybackType::NONE;
//
//	UPROPERTY()
//	TArray<FDotModeObject> PointList;
//
//	void to_json(FJsonObject& j)
//	{
//		FString playbackValue = "";
//		TArray <TSharedPtr<FJsonValue>> pointValues;
//
//		switch (EPlaybackType)
//		{
//		case EPlaybackType::NONE:
//			playbackValue = "NONE";
//			break;
//		case EPlaybackType::FADE_IN:
//			playbackValue = "FADE_IN";
//			break;
//		case EPlaybackType::FADE_OUT:
//			playbackValue = "FADE_OUT";
//			break;
//		case EPlaybackType::FADE_IN_OUT:
//			playbackValue = "FADE_IN_OUT";
//			break;
//		default:
//			playbackValue = "ERROR";
//			break;
//		}
//
//		for (int i = 0; i < PointList.Num(); i++)
//		{
//			TSharedPtr<FJsonObject> pointTemp = MakeShareable(new FJsonObject);
//			PointList[i].to_json(*pointTemp);
//			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(pointTemp));
//			pointValues.Add(JsonValue);
//		}
//
//		j.SetNumberField("StartTime", StartTime);
//		j.SetNumberField("EndTime", EndTime);
//		j.SetStringField("EPlaybackType", playbackValue);
//		j.SetArrayField("PointList", pointValues);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> dotObj = j.GetArrayField("pointList");
//		FString playbackType = j.GetStringField(TEXT("playbackType"));
//
//		StartTime = j.GetIntegerField("startTime");
//		EndTime = j.GetIntegerField("endTime");
//
//		if (playbackType == "NONE")
//		{
//			PlaybackType = EPlaybackType::NONE;
//		}
//		else if (playbackType == "FADE_IN")
//		{
//			EPlaybackType = EPlaybackType::FADE_IN;
//		}
//		else if (playbackType == "FADE_OUT")
//		{
//			EPlaybackType = EPlaybackType::FADE_OUT;
//		}
//		else
//		{
//			EPlaybackType = EPlaybackType::FADE_IN_OUT;
//		}
//
//		for (int i = 0; i < dotObj.Num(); i++)
//		{
//			FDotModeObject tempDot;
//			tempDot.from_json(*dotObj[i]->AsObject());
//			PointList.Add(tempDot);
//		}
//
//	}
//};
//
//USTRUCT()
//struct FDotMode
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//		bool DotConnected;
//
//	UPROPERTY()
//	TArray<FDotModeObjectCollection> Feedback;
//
//	void to_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> feedbackValues;
//
//		for (int i = 0; i < Feedback.Num(); i++)
//		{
//			TSharedPtr<FJsonObject> dotObjectTemp = MakeShareable(new FJsonObject);
//			Feedback[i].to_json(*dotObjectTemp);
//			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(dotObjectTemp));
//			feedbackValues.Add(JsonValue);
//		}
//
//		j.SetBoolField("DotConnected", DotConnected);
//		j.SetArrayField("Feedback", feedbackValues);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> feedbackObj = j.GetArrayField("feedback");
//
//		DotConnected = j.GetBoolField("dotConnected");
//
//		for (int i = 0; i < feedbackObj.Num(); i++)
//		{
//			FDotModeObjectCollection dotCollectionTemp;
//			dotCollectionTemp.from_json(*feedbackObj[i]->AsObject());
//			Feedback.Add(dotCollectionTemp);
//		}
//	}
//
//};
//
//USTRUCT()
//struct FPathModeObject
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//		float X;
//
//	UPROPERTY()
//	float Y;
//
//	UPROPERTY()
//	float Intensity;
//
//	UPROPERTY()
//	int Time;
//
//	void to_json(FJsonObject& j)
//	{
//		j.SetStringField("X", FString::SanitizeFloat(X));
//		j.SetStringField("Y", FString::SanitizeFloat(Y));
//		j.SetStringField("Intensity", FString::SanitizeFloat(Intensity));
//
//		j.SetNumberField("Time", Time);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		X = j.GetNumberField("x");
//		Y = j.GetNumberField("y");
//		Intensity = j.GetNumberField("intensity");
//		Time = j.GetIntegerField("time");
//	}
//};
//
//USTRUCT()
//struct FPathModeObjectCollection
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//	EPlaybackType PlaybackType = EPlaybackType::NONE;
//
//	UPROPERTY()
//	EPathMovingPattern MovingPattern = EPathMovingPattern::CONST_TDM;
//
//	UPROPERTY()
//	TArray<FPathModeObject> PointList;
//
//
//	void to_json(FJsonObject& j)
//	{
//		FString playbackValue = "";
//		FString patternValue = "";
//		TArray<TSharedPtr<FJsonValue>> pointValues;
//
//		switch (EPlaybackType)
//		{
//		case EPlaybackType::NONE:
//			playbackValue = "NONE";
//			break;
//		case EPlaybackType::FADE_IN:
//			playbackValue = "FADE_IN";
//			break;
//		case EPlaybackType::FADE_OUT:
//			playbackValue = "FADE_OUT";
//			break;
//		case EPlaybackType::FADE_IN_OUT:
//			playbackValue = "FADE_IN_OUT";
//			break;
//		default:
//			playbackValue = "ERROR";
//			break;
//		}
//
//		switch (MovingPattern)
//		{
//		case EPathMovingPattern::CONST_TDM:
//			patternValue = "CONST_TDM";
//			break;
//		case EPathMovingPattern::CONST_SPEED:
//			patternValue = "CONST_SPEED";
//			break;
//		default:
//			patternValue = "ERROR";
//			break;
//		}
//
//		j.SetStringField("EPlaybackType", playbackValue);
//		j.SetStringField("MovingPattern", patternValue);
//
//		for (int i = 0; i < PointList.Num(); i++)
//		{
//			TSharedPtr<FJsonObject> pointTemp = MakeShareable(new FJsonObject);
//			PointList[i].to_json(*pointTemp);
//			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(pointTemp));
//			pointValues.Add(JsonValue);
//		}
//
//		j.SetArrayField("PointList", pointValues);
//
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		FString playbackType = j.GetStringField(TEXT("playbackType"));
//		FString movingPattern = j.GetStringField(TEXT("movingPattern"));
//
//		if (playbackType == "NONE")
//		{
//			PlaybackType = EPlaybackType::NONE;
//		}
//		else if (playbackType == "FADE_IN")
//		{
//			PlaybackType = EPlaybackType::FADE_IN;
//		}
//		else if (playbackType == "FADE_OUT")
//		{
//			PlaybackType = EPlaybackType::FADE_OUT;
//		}
//		else
//		{
//			PlaybackType = EPlaybackType::FADE_IN_OUT;
//		}
//
//		if (movingPattern == "CONST_SPEED")
//		{
//			MovingPattern = EPathMovingPattern::CONST_SPEED;
//		}
//		else
//		{
//			MovingPattern = EPathMovingPattern::CONST_TDM;
//		}
//
//		TArray<TSharedPtr<FJsonValue>> pathObj = j.GetArrayField("pointList");
//		for (int l = 0; l < pathObj.Num(); l++)
//		{
//			FPathModeObject tempDot;
//			tempDot.from_json(*pathObj[l]->AsObject());
//			PointList.Add(tempDot);
//		}
//	}
//};
//
//USTRUCT()
//struct FPathMode
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	TArray<FPathModeObjectCollection> Feedback;
//
//	void to_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> feedbackValues;
//
//		for (int i = 0; i < Feedback.Num(); i++)
//		{
//			TSharedPtr<FJsonObject> pathmodeTemp = MakeShareable(new FJsonObject);
//			Feedback[i].to_json(*pathmodeTemp);
//			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(pathmodeTemp));
//
//			feedbackValues.Add(JsonValue);
//		}
//		j.SetArrayField("Feedback", feedbackValues);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> feedbackObj = j.GetArrayField("feedback");
//
//		for (int i = 0; i < feedbackObj.Num(); i++)
//		{
//			FPathModeObjectCollection pathCollectionTemp;
//			pathCollectionTemp.from_json(*feedbackObj[i]->AsObject());
//			Feedback.Add(pathCollectionTemp);
//		}
//	}
//};
//
//USTRUCT()
//struct FHapticEffectMode
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//	int Texture;
//
//	UPROPERTY()
//	EFeedbackMode Mode;
//
//	UPROPERTY()
//	FDotMode DotMode;
//
//	UPROPERTY()
//	FPathMode PathMode;
//
//	void to_json(FJsonObject& j)
//	{
//		FString modeValue = "";
//		TSharedPtr<FJsonObject> dotModeValue = MakeShareable(new FJsonObject);
//		TSharedPtr<FJsonObject> pathModeValue = MakeShareable(new FJsonObject);
//
//		switch (Mode)
//		{
//		case PATH_MODE:
//			modeValue = "PATH_MODE";
//			break;
//		case DOT_MODE:
//			modeValue = "DOT_MODE";
//			break;
//		default:
//			modeValue = "ERROR";
//			break;
//		}
//
//		DotMode.to_json(*dotModeValue);
//		PathMode.to_json(*pathModeValue);
//
//		j.SetNumberField("Texture", Texture);
//		j.SetStringField("Mode", modeValue);
//		j.SetObjectField("DotMode", dotModeValue);
//		j.SetObjectField("PathMode", pathModeValue);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		FString modeEnum = j.GetStringField(TEXT("mode"));
//
//		if (modeEnum == "PATH_MODE")
//		{
//			Mode = PATH_MODE;
//		}
//		else if (modeEnum == "DOT_MODE")
//		{
//			Mode = DOT_MODE;
//		}
//
//
//		Texture = j.GetNumberField("texture");
//		DotMode.from_json(*j.GetObjectField("dotMode"));
//		PathMode.from_json(*j.GetObjectField("pathMode"));
//
//	}
//};
//
//USTRUCT()
//struct FHapticEffect
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//	int StartTime;
//
//	UPROPERTY()
//	int OffsetTime;
//
//	UPROPERTY()
//	TMap<FString, FHapticEffectMode> Modes;
//
//	void to_json(FJsonObject& j)
//	{
//		TSharedPtr<FJsonObject> modeObject = MakeShareable(new FJsonObject);
//
//		for (auto& m : Modes)
//		{
//			TSharedPtr<FJsonObject> effectObject = MakeShareable(new FJsonObject);
//
//			m.Value.to_json(*effectObject);
//			modeObject->SetObjectField(m.Key, effectObject);
//		}
//
//		j.SetNumberField("StartTime", StartTime);
//		j.SetNumberField("OffsetTime", OffsetTime);
//		j.SetObjectField("Modes", modeObject);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		TMap<FString, TSharedPtr<FJsonValue>> modeValues = j.GetObjectField("modes")->Values;
//
//		for (auto& kv : modeValues)
//		{
//			FString key = kv.Key;
//			FHapticEffectMode tempMode;
//			tempMode.from_json(*kv.Value->AsObject());
//			//Modes[key] = tempMode;
//			Modes.Add(key, tempMode);
//		}
//
//		StartTime = j.GetIntegerField("startTime");
//		OffsetTime = j.GetIntegerField("offsetTime");
//
//	}
//
//};
//
//USTRUCT()
//struct FTrack
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	TArray<FHapticEffect> Effects;
//
//	void to_json(FJsonObject& j)
//	{
//
//		TArray<TSharedPtr<FJsonValue>> effectValues;
//
//		for (int i = 0; i < Effects.Num(); i++)
//		{
//			TSharedPtr<FJsonObject> effectObject = MakeShareable(new FJsonObject);
//			Effects[i].to_json(*effectObject);
//			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(effectObject));
//			effectValues.Add(JsonValue);
//		}
//
//		j.SetArrayField("Effects", effectValues);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> effectObject = j.GetArrayField("effects");
//
//		for (int i = 0; i < effectObject.Num(); i++)
//		{
//			FHapticEffect tempEffect;
//			tempEffect.from_json(*effectObject[i]->AsObject());
//			Effects.Add(tempEffect);
//		}
//	}
//};
//
//USTRUCT()
//struct FLayoutObject
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	int Index;
//
//	UPROPERTY()
//	float X;
//
//	UPROPERTY()
//	float Y;
//
//	void to_json(FJsonObject& j)
//	{
//		j.SetNumberField("Index", Index);
//		j.SetStringField("X", FString::SanitizeFloat(X));
//		j.SetStringField("Y", FString::SanitizeFloat(Y));
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		Index = j.GetIntegerField("index");
//		X = j.GetNumberField("x");
//		Y = j.GetNumberField("y");
//	}
//
//};
//
//USTRUCT()
//struct FLayoutObjects
//{
//	GENERATED_BODY()
//
//		UPROPERTY()
//		TArray<FLayoutObject> LayoutObjects;
//
//	void to_json(TArray<TSharedPtr<FJsonObject>> &jVec)
//	{
//		for (int i = 0; i < LayoutObjects.Num(); i++)
//		{
//			TSharedPtr<FJsonObject> layoutTemp = MakeShareable(new FJsonObject);
//			LayoutObjects[i].to_json(*layoutTemp);
//			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(layoutTemp));
//			jVec.Add(JsonValue);
//		}
//	}
//};
//
//USTRUCT()
//struct FLayout
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	FString Type;
//
//	UPROPERTY()
//	TMap<FString, FLayoutObjects> Layouts;
//
//	void to_json(FJsonObject& j)
//	{
//		TSharedPtr<FJsonObject> layout = MakeShareable(new FJsonObject);
//
//		for (auto& l : Layouts)
//		{
//			TArray<TSharedPtr<FJsonValue>> jVec;
//			l.Value.to_json(jVec);
//			layout->SetArrayField(l.Key, jVec);
//		}
//		j.SetStringField("Type", Type);
//		j.SetObjectField("Layouts", layout);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//
//		TMap<FString, TSharedPtr<FJsonValue>> layoutsObj = j.GetObjectField("layouts")->Values;
//		Type = TCHAR_TO_UTF8(*j.GetStringField(TEXT("type")));
//
//		for (auto& kv : layoutsObj)
//		{
//			FString key = kv.Key;
//			TArray<TSharedPtr<FJsonValue>> layoutObjectValues = kv.Value->AsArray();
//			TArray<FLayoutObject> tempLayoutObject;
//
//			for (int i = 0; i < layoutObjectValues.Num(); i++)
//			{
//				FLayoutObject tempLayout;
//				tempLayout.from_json(*layoutObjectValues[i]->AsObject());
//				tempLayoutObject.Add(tempLayout);
//			}
//			//Layouts[key] = tempLayoutObject;
//
//			FLayoutObjects tempObjects;
//			tempObjects.LayoutObjects =  tempLayoutObject;
//			Layouts.Add(key, tempObjects);
//		}
//
//	}
//};
//
//USTRUCT()
//struct FHapticProject
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	TArray<FTrack> Tracks;
//	UPROPERTY()
//	FLayout Layout;
//
//	void to_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> trackValues;
//		TSharedPtr<FJsonObject> layoutObject = MakeShareable(new FJsonObject);
//
//		for (int i = 0; i < Tracks.Num(); i++)
//		{
//			TSharedPtr<FJsonObject> trackObject = MakeShareable(new FJsonObject);
//			Tracks[i].to_json(*trackObject);
//			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(trackObject));
//			trackValues.Add(JsonValue);
//		}
//
//		Layout.to_json(*layoutObject);
//
//		j.SetArrayField("Tracks", trackValues);
//		j.SetObjectField("Layout", layoutObject);
//	}
//
//	void from_json(FJsonObject& j)
//	{
//		TArray<TSharedPtr<FJsonValue>> trackObjs = j.GetArrayField(TEXT("Tracks"));
//
//		for (int i = 0; i < trackObjs.Num(); i++)
//		{
//			FTrack tempTrack;
//			tempTrack.from_json(*trackObjs[i]->AsObject());
//			Tracks.Add(tempTrack);
//		}
//		Layout.from_json(*j.GetObjectField("layout"));
//	}
//};

class HAPTICSMANAGER_API HapticStructures
{
public:
	HapticStructures();
	~HapticStructures();
};