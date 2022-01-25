// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AndroidHapticLibrary.h"
#include "BhapticsUtils.h"
#include "HapticsManager.h"
#include "JsonObjectConverter.h"
#include "Json/Public/Serialization/JsonWriter.h"
#include "Json/Public/Policies/CondensedJsonPrintPolicy.h"

FPlayerResponse UAndroidHapticLibrary::CurrentResponse;
FPlayerResponse UAndroidHapticLibrary::LastUpdatedResponse;
TArray< FDevice> UAndroidHapticLibrary::DeviceList;

#if PLATFORM_ANDROID
jmethodID GetCurrentDeviceMethod;
jmethodID TurnOffMethodId;
jmethodID TurnOffAllMethodId;
jmethodID GetPositionStatusMethodId;
jmethodID PingMethodId;
jmethodID PingAllMethodId;
jmethodID PingPositionMethodId;
#endif

UAndroidHapticLibrary::UAndroidHapticLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		GetCurrentDeviceMethod = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_getDeviceList", "()Ljava/lang/String;", false);
		
		TurnOffMethodId =FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_TurnOff", "(Ljava/lang/String;)V", false);

		TurnOffAllMethodId =FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_TurnOffAll", "()V", false);

		GetPositionStatusMethodId = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetPositionStatus", "(Ljava/lang/String;)[B", false);

		PingMethodId = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Ping", "(Ljava/lang/String;)V", false);

		PingPositionMethodId = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PingPosition", "(Ljava/lang/String;)V", false);

		PingAllMethodId = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PingAll", "()V", false);
	}
#endif
}

int64 last = 0;
TArray<FDevice> UAndroidHapticLibrary::GetCurrentDevices()
{
	FDateTime Time = FDateTime::Now();
	//Get the timestamp
	int64 Timestamp = (int64) (Time.GetTicks() / ETimespan::TicksPerMillisecond);

	if (Timestamp - last < 1000) {
		return DeviceList;
	}

	last = Timestamp;


	TArray<FDevice> UpdateDeviceList;
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jstr = (jstring) FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, GetCurrentDeviceMethod);
		const char* nativeDeviceListString = Env->GetStringUTFChars(jstr, 0);
		FString DevicesListString = FString(nativeDeviceListString);

		Env->ReleaseStringUTFChars(jstr, nativeDeviceListString);

		if (!FJsonObjectConverter::JsonArrayStringToUStruct(DevicesListString, &UpdateDeviceList, 0, 0))
		{
			UE_LOG(LogTemp, Log, TEXT("parse failed"));
		}
	}

#else
	FDevice d, d2, d3, d4, d5, d6, d7, d8;
	d.Battery = 10;
	d.Position = "ForearmL";
	d.IsConnected = true;
	d.IsAudioJackIn = false;
	d.Battery = 30;

	d8.Position = "GloveL";
	d8.IsConnected = true;
	d8.IsAudioJackIn = false;
	d8.Battery= 40;

	d2.Position = "Vest";
	d2.IsConnected = true;
	d2.IsAudioJackIn = true;
	d2.Battery= 10;

	d6.Battery = 10;
	d6.Position = "HandR";
	d6.IsConnected = true;
	d6.IsAudioJackIn = false;
	d6.Battery = 30;

	d7.Battery = 10;
	d7.Position = "FootL";
	d7.IsConnected = true;
	d7.IsAudioJackIn = false;
	d7.Battery = 30;

	d5.Position = "Vest";
	d5.IsConnected = true;
	d5.IsAudioJackIn = false;
	d5.Battery= 30;

	d3.Position = "Head";
	d3.IsConnected = false;
	d3.Battery= -1;

	d4.Position = "HandR";
	d4.IsConnected = false;
	d4.Battery=-1;
	//UpdateDeviceList.Add(d);
	//DeviceList.Add(d5);
	//DeviceList.Add(d6);
	//DeviceList.Add(d7);
	//UpdateDeviceList.Add(d2);
	//UpdateDeviceList.Add(d3);
	//UpdateDeviceList.Add(d4);
	//UpdateDeviceList.Add(d8);
#endif
	DeviceList = UpdateDeviceList;
	return UpdateDeviceList;
}

void UAndroidHapticLibrary::AndroidThunkCpp_Ping(FString DeviceAddress)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring DeviceAddressJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DeviceAddress));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PingMethodId, DeviceAddressJava);
		Env->DeleteLocalRef(DeviceAddressJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkJava_PingPosition(FString Position)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring PositionJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Position));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PingPositionMethodId, PositionJava);
		Env->DeleteLocalRef(PositionJava);
	}
#endif // PLATFORM_ANDROID
}


FString PosToString(EPosition Pos)
{
	switch (Pos)
	{
	case EPosition::VestFront:
		return "Vest";
	case EPosition::VestBack:
		return "Vest";

	case EPosition::Head:
		return "Head";
	case EPosition::Left:
	case EPosition::ForearmL:
		return "ForearmL";
	case EPosition::Right:
	case EPosition::ForearmR:
		return "ForearmR";
	case EPosition::GloveL:
		return "GloveL";
	case EPosition::GloveR:
		return "GloveR";
	case EPosition::HandL:
		return "HandL";
	case EPosition::HandR:
		return "HandR";
	case EPosition::FootL:
		return "FootL";
	case EPosition::FootR:
		return "FootR";
	default:
		break;
	}

	return "Unknown";
}


bool UAndroidHapticLibrary::IsDeviceConnceted(EPosition Position)
{
	auto Devices = UAndroidHapticLibrary::GetCurrentDevices();
	for (int i = 0; i < Devices.Num(); i++) {
		FDevice d = Devices[i];
		if (d.IsConnected && PosToString(Position) == d.Position) {
			return true;
		}
	}
	return false;
}

void UAndroidHapticLibrary::AndroidThunkCpp_PingAll()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PingAllMethodId);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_ChangePosition(FString DeviceAddress, FString Position)
{
	UE_LOG(LogTemp, Log, TEXT("AndroidThunkCpp_ChangePosition"));
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID ChangePositionMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ChangePosition", "(Ljava/lang/String;Ljava/lang/String;)V", false);
		jstring DeviceAddressJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DeviceAddress));
		jstring PositionJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Position));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ChangePositionMethod, DeviceAddressJava, PositionJava);
		Env->DeleteLocalRef(DeviceAddressJava);
		Env->DeleteLocalRef(PositionJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_TogglePosition(FString DeviceAddress)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID TogglePositionMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_TogglePosition", "(Ljava/lang/String;)V", false);
		jstring DeviceAddressJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DeviceAddress));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, TogglePositionMethod,DeviceAddressJava);
		Env->DeleteLocalRef(DeviceAddressJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::SubmitDot(FString Key, FString Pos, TArray<FDotPoint> Points, int DurationMillis)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		UE_LOG(LogTemp, Log, TEXT("SubmitDot"));
		static jmethodID submitDotMethodId =
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID,
				"AndroidThunkJava_SubmitDot", "(Ljava/lang/String;Ljava/lang/String;[I[II)V", false);
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Key));
		jstring posJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Pos));

		jintArray indexesJava = Env->NewIntArray(Points.Num());
		jintArray intensitiesJava = Env->NewIntArray(Points.Num());

		jint* indexes = new jint[Points.Num()]; 
		jint* intensities = new jint[Points.Num()];
		for (int i = 0; i < Points.Num(); ++i) {
			indexes[i] = Points[i].Index;
			intensities[i] = Points[i].Intensity;
		}
		Env->SetIntArrayRegion(indexesJava, 0, Points.Num(), indexes);
		Env->SetIntArrayRegion(intensitiesJava, 0, Points.Num(), intensities);

		FJavaWrapper::CallVoidMethod(
			Env, FJavaWrapper::GameActivityThis, submitDotMethodId, 
			keyStrJava, posJava, indexesJava, intensitiesJava,  DurationMillis);
		Env->DeleteLocalRef(keyStrJava);
		Env->DeleteLocalRef(posJava);


		delete[] indexes;
		delete[] intensities;

	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::SubmitPath(FString Key, FString Pos, TArray<FPathPoint> Points, int DurationMillis)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		UE_LOG(LogTemp, Log, TEXT("SubmitPath"));
		static jmethodID submitDotMethodId =
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID,
				"AndroidThunkJava_SubmitPath", "(Ljava/lang/String;Ljava/lang/String;[F[F[II)V", false);
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Key));
		jstring posJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Pos));

		jfloatArray xJava = Env->NewFloatArray(Points.Num());
		jfloatArray yJava = Env->NewFloatArray(Points.Num());
		jintArray intensitiesJava = Env->NewIntArray(Points.Num());
		jfloat* x = new jfloat[Points.Num()];
		jfloat* y = new jfloat[Points.Num()];
		jint* intensities = new jint[Points.Num()];
		for (int i = 0; i < Points.Num(); ++i) {
			x[i] = Points[i].X;
			y[i] = Points[i].Y;
			intensities[i] = Points[i].Intensity;
		}

		Env->SetFloatArrayRegion(xJava, 0, Points.Num(), x);
		Env->SetFloatArrayRegion(yJava, 0, Points.Num(), y);
		Env->SetIntArrayRegion(intensitiesJava, 0, Points.Num(), intensities);


		FJavaWrapper::CallVoidMethod(
			Env, FJavaWrapper::GameActivityThis, submitDotMethodId,
			keyStrJava, posJava, xJava, yJava, intensitiesJava, DurationMillis);
		Env->DeleteLocalRef(keyStrJava);
		Env->DeleteLocalRef(posJava);

		delete[] x;
		delete[] y;
		delete[] intensities;
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::TurnOff(FString Key)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Key));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, TurnOffMethodId, keyStrJava);
		Env->DeleteLocalRef(keyStrJava);
}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::TurnOffAll()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, TurnOffAllMethodId);
	}
#endif // PLATFORM_ANDROID
}

bool UAndroidHapticLibrary::IsDeviceConnceted(FString Position)
{
	auto Devices = UAndroidHapticLibrary::GetCurrentDevices();
	for (int i = 0; i < Devices.Num(); i++) {
		FDevice d = Devices[i];
		if (d.IsConnected
			&& Position == d.Position) {
			return true;
		}
	}
	return false;
}

bool UAndroidHapticLibrary::IsFeedbackRegistered(FString key)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID isRegisterMethodId = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsRegistered", "(Ljava/lang/String;)Z", false);
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*key));
		bool res = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, isRegisterMethodId, keyStrJava);
		Env->DeleteLocalRef(keyStrJava);

		return res;
	}
#endif // PLATFORM_ANDROID

	return false;
}

bool UAndroidHapticLibrary::IsFeedbackPlaying(FString key)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID isRegisterMethodId = 
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsPlaying", "(Ljava/lang/String;)Z", false);
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*key));
		bool res = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, isRegisterMethodId, keyStrJava);
		Env->DeleteLocalRef(keyStrJava);

		return res;
	}
#endif // PLATFORM_ANDROID

	return false;
}

bool UAndroidHapticLibrary::IsAnyFeedbackPlaying()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID isRegisterMethodId =
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsAnythingPlaying", "()Z", false);
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, isRegisterMethodId);
	}
#endif // PLATFORM_ANDROID
	return false;
}

TArray<uint8> UAndroidHapticLibrary::GetPositionStatus(FString pos)
{
	TArray<uint8> IntArray;
	IntArray.Init(0, 20);
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring posJava = Env->NewStringUTF(TCHAR_TO_UTF8(*pos));
	
		jbyteArray arrayJava = (jbyteArray) FJavaWrapper::CallObjectMethod(
			Env, FJavaWrapper::GameActivityThis, GetPositionStatusMethodId, posJava);

		Env->DeleteLocalRef(posJava);

		jbyte* byteArr = Env->GetByteArrayElements(arrayJava, 0);
		jsize length = Env->GetArrayLength(arrayJava);
		for (int posIndex = 0; posIndex < length; posIndex++)
		{
			IntArray[posIndex] = byteArr[posIndex];
		}

	}
#endif // PLATFORM_ANDROID

	return IntArray;
}

void UAndroidHapticLibrary::RegisterProject(FString key, FString fileStr)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID registerMethodId = 
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, 
				"AndroidThunkJava_Register", 
				"(Ljava/lang/String;Ljava/lang/String;)V", false);
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*key));
		jstring fileStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*fileStr));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, registerMethodId, keyStrJava, fileStrJava);
		Env->DeleteLocalRef(keyStrJava);
		Env->DeleteLocalRef(fileStrJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::RegisterProjectReflected(FString key, FString fileStr)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID registerMethodId =
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID,
				"AndroidThunkJava_RegisterReflected",
				"(Ljava/lang/String;Ljava/lang/String;)V", false);
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*key));
		jstring fileStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*fileStr));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, registerMethodId, keyStrJava, fileStrJava);
		Env->DeleteLocalRef(keyStrJava);
		Env->DeleteLocalRef(fileStrJava);
	}
#endif // PLATFORM_ANDROID
}

bool UAndroidHapticLibrary::AndroidThunkCpp_Initialize(FString appName)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID InitializeMethodId = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID,
			"AndroidThunkJava_Initialize", "(Ljava/lang/String;)V", false);
		jstring appNameJava = Env->NewStringUTF(TCHAR_TO_UTF8(*appName));
		FJavaWrapper::CallVoidMethod(
			Env, FJavaWrapper::GameActivityThis, InitializeMethodId, appNameJava);
		Env->DeleteLocalRef(appNameJava);
	}
#endif // PLATFORM_ANDROID
	return true;
}

void UAndroidHapticLibrary::SubmitRegistered(
	FString key, FString altKey, float intensity, float duration, float xOffsetAngle, float yOffset)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FString alt = altKey;
		if (altKey.IsEmpty()) {
			alt = key;
		}

		static jmethodID SubmitMethod = FJavaWrapper::FindMethod(
			Env, FJavaWrapper::GameActivityClassID, 
				"AndroidThunkJava_SubmitRegistered", "(Ljava/lang/String;Ljava/lang/String;FFFF)V", false);
		jstring keyJava = Env->NewStringUTF(TCHAR_TO_UTF8(*key));
		jstring altKeyJava = Env->NewStringUTF(TCHAR_TO_UTF8(*alt));
		FJavaWrapper::CallVoidMethod(
			Env, FJavaWrapper::GameActivityThis, SubmitMethod, keyJava, altKeyJava,
			intensity, duration, xOffsetAngle, yOffset);
		Env->DeleteLocalRef(keyJava);
		Env->DeleteLocalRef(altKeyJava);
	}
#endif // PLATFORM_ANDROID
}