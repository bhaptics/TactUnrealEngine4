// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AndroidHapticLibrary.h"
#include "BhapticsUtils.h"
#include "HapticsManager.h"
#include "JsonObjectConverter.h"
#include "Json/Public/Serialization/JsonWriter.h"
#include "Json/Public/Policies/CondensedJsonPrintPolicy.h"

TArray<FDevice> UAndroidHapticLibrary::FoundDevices;
FPlayerResponse UAndroidHapticLibrary::CurrentResponse;
FPlayerResponse UAndroidHapticLibrary::LastUpdatedResponse;

FDeviceArrayDelegate UAndroidHapticLibrary::UpdateDeviceListDelegate;
FDeviceStatusDelegate UAndroidHapticLibrary::UpdateDeviceStatusDelegate;

FCriticalSection UAndroidHapticLibrary::m_Mutex;

UAndroidHapticLibrary::UAndroidHapticLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

#if  PLATFORM_ANDROID
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeOnDeviceFound(JNIEnv* jenv, jobject thiz, jstring deviceListString)
{
	const char *nativeDeviceListString = jenv->GetStringUTFChars(deviceListString, 0);
	FString DevicesListString = FString(nativeDeviceListString);
	jenv->ReleaseStringUTFChars(deviceListString, nativeDeviceListString);
	TArray<FDevice> DeviceList;
	if (FJsonObjectConverter::JsonArrayStringToUStruct(DevicesListString, &DeviceList, 0, 0))
	{
		UAndroidHapticLibrary::UpdateDevices(DeviceList);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("parse failed"));
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeOnChangeScanState(JNIEnv* jenv, jobject thiz)
{
	UE_LOG(LogTemp, Log, TEXT("The current scanning state"));
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeOnChangeResponse(JNIEnv* jenv, jobject thiz)
{
	// TODO
}

#endif //  PLATFORM_ANDROID

void UAndroidHapticLibrary::UpdateDevices(TArray<FDevice> DeviceList)
{
	FoundDevices = DeviceList;
	UpdateDeviceListDelegate.ExecuteIfBound(DeviceList);
}

void UAndroidHapticLibrary::ParsePlayerResponse(FString ResponseString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Deserialisation failed."));
	}
	else
	{
		FPlayerResponse Response = FPlayerResponse();

		TArray<TSharedPtr<FJsonValue>> RegisteredKeysValueArray = JsonObject->GetArrayField("RegisteredKeys");
		TArray<TSharedPtr<FJsonValue>> ActiveKeysValueArray = JsonObject->GetArrayField("ActiveKeys");
		Response.ConnectedDeviceCount = JsonObject->GetIntegerField("ConnectedDeviceCount");
		TArray<TSharedPtr<FJsonValue>> ConnectedPositionsValueArray = JsonObject->GetArrayField("ConnectedPositions");
		TMap<FString, TSharedPtr<FJsonValue>> StatusValuesMap = JsonObject->GetObjectField("Status")->Values;
		int i = 0;

		for (i = 0; i < RegisteredKeysValueArray.Num(); i++)
		{
			Response.RegisteredKeys.Add(RegisteredKeysValueArray[i]->AsString());
		}

		for (i = 0; i < ActiveKeysValueArray.Num(); i++)
		{
			Response.ActiveKeys.Add(ActiveKeysValueArray[i]->AsString());
		}

		for (i = 0; i < ConnectedPositionsValueArray.Num(); i++)
		{
			Response.ConnectedPositions.Add(ConnectedPositionsValueArray[i]->AsString());
		}
		
		TArray<FHapticFeedback> MotorsToUpdate;
		for (auto& StatusValue : StatusValuesMap)
		{
			FString PositionName = StatusValue.Key;
			TArray<TSharedPtr<FJsonValue>> MotorValues = StatusValue.Value->AsArray();
			TArray<uint8> Motors = TArray<uint8>();
			for (i = 0; i < MotorValues.Num(); i++)
			{
				Motors.Add(MotorValues[i]->AsNumber());
			}
			MotorsToUpdate.Add(FHapticFeedback(StringToPosition(PositionName), Motors, EFeedbackMode::PATH_MODE));
			
		}
		Response.Status = MotorsToUpdate;
		m_Mutex.Lock();
		CurrentResponse = Response;
		m_Mutex.Unlock();

		UpdateDeviceStatusDelegate.ExecuteIfBound(MotorsToUpdate);
	}
}

FPlayerResponse UAndroidHapticLibrary::GetCurrentResponse()
{
	if (m_Mutex.TryLock())
	{
		LastUpdatedResponse = CurrentResponse;
		m_Mutex.Unlock();
	}
	return LastUpdatedResponse;
}

void UAndroidHapticLibrary::AndroidThunkCpp_StartScanning()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID StartScanMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Scan", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, StartScanMethod);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_StopScanning()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID StopScanMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StopScan", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, StopScanMethod);
	}
#endif // PLATFORM_ANDROID
}

bool UAndroidHapticLibrary::AndroidThunkCpp_IsScanning()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID IsScanningMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsScanning", "()Z", false);
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, IsScanningMethod);
	}
#endif // PLATFORM_ANDROID
	return false;
}

void UAndroidHapticLibrary::AndroidThunkCpp_Pair(FString DeviceAddress)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID PairMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Pair", "(Ljava/lang/String;)V", false);
		jstring DeviceAddressJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DeviceAddress));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PairMethod, DeviceAddressJava);
		Env->DeleteLocalRef(DeviceAddressJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_PairFromPosition(FString DeviceAddress, FString DevicePosition)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID PairMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PairFromPosition", "(Ljava/lang/String;Ljava/lang/String;)V", false);
		jstring DeviceAddressJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DeviceAddress));
		jstring DevicePositionJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DevicePosition));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PairMethod, DeviceAddressJava, DevicePositionJava);
		Env->DeleteLocalRef(DeviceAddressJava);
		Env->DeleteLocalRef(DevicePositionJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_Unpair(FString DeviceAddress)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID UnpairMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Unpair", "(Ljava/lang/String;)V", false);
		jstring DeviceAddressJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DeviceAddress));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UnpairMethod,DeviceAddressJava);
		Env->DeleteLocalRef(DeviceAddressJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_UnpairAll()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID UnpairAllMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_UnpairAll", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UnpairAllMethod);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_Ping(FString DeviceAddress)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID PingMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Ping", "(Ljava/lang/String;)V", false);
		jstring DeviceAddressJava = Env->NewStringUTF(TCHAR_TO_UTF8(*DeviceAddress));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PingMethod, DeviceAddressJava);
		Env->DeleteLocalRef(DeviceAddressJava);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_PingAll()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID PingAllMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PingAll", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PingAllMethod);
	}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::AndroidThunkCpp_ChangePosition(FString DeviceAddress, FString Position)
{
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


EPosition UAndroidHapticLibrary::StringToPosition(FString PositionString)
{
	EPosition ReturnValue = EPosition::Default;

	if (PositionString == "Left") {
		ReturnValue = EPosition::Left;
	}
	else if (PositionString == "Right")
	{
		ReturnValue = EPosition::Right;
	}
	else if (PositionString == "ForearmL")
	{
		ReturnValue = EPosition::ForearmL;
	}
	else if (PositionString == "ForearmR")
	{
		ReturnValue = EPosition::ForearmR;
	}
	else if (PositionString == "VestFront")
	{
		ReturnValue = EPosition::VestFront;
	}
	else if (PositionString == "VestBack")
	{
		ReturnValue = EPosition::VestBack;
	}
	else if (PositionString == "Head")
	{
		ReturnValue = EPosition::Head;
	}
	else if (PositionString == "HandL")
	{
		ReturnValue = EPosition::HandL;
	}
	else if (PositionString == "HandR")
	{
		ReturnValue = EPosition::HandR;
	}
	else if (PositionString == "FootL")
	{
		ReturnValue = EPosition::FootL;
	}
	else if (PositionString == "FootR")
	{
		ReturnValue = EPosition::FootR;
	}
	
	return ReturnValue;
}

void UAndroidHapticLibrary::SubmitDot(FString Key, FString Pos, TArray<FDotPoint> Points, int DurationMillis)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
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
		static jmethodID isRegisterMethodId = 
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_TurnOff", "(Ljava/lang/String;)V", false);
		jstring keyStrJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Key));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, isRegisterMethodId, keyStrJava);
		Env->DeleteLocalRef(keyStrJava);
}
#endif // PLATFORM_ANDROID
}

void UAndroidHapticLibrary::TurnOffAll()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID isRegisterMethodId = 
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_TurnOffAll", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, isRegisterMethodId);
	}
#endif // PLATFORM_ANDROID
}

bool UAndroidHapticLibrary::IsDeviceConnceted(EPosition Position)
{
	for (int i = 0; i < FoundDevices.Num(); i++) {
		FDevice d = FoundDevices[i];
		if (d.ConnectionStatus == "Connected" 
			&& BhapticsUtils::PositionEnumToString(Position) == d.Position) {
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
		

		static jmethodID isRegisterMethodId =
			FJavaWrapper::FindMethod(
				Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetPositionStatus", "(Ljava/lang/String;)[B", false);
		jbyteArray arrayJava = (jbyteArray) FJavaWrapper::CallObjectMethod(
			Env, FJavaWrapper::GameActivityThis, isRegisterMethodId, posJava);

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