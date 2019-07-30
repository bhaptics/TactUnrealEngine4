// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AndroidHapticLibrary.h"
#include "HapticsManager.h"
#include "JsonUtilities/Public/JsonUtilities.h"

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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeOnChangeScanState(JNIEnv* jenv, jobject thiz, jstring scanningState)
{
	const char *nativeStateString = jenv->GetStringUTFChars(scanningState, 0);
	FString StateString = FString(nativeStateString);
	jenv->ReleaseStringUTFChars(scanningState, nativeStateString);
	UE_LOG(LogTemp, Log, TEXT("The current state is %s"), *StateString);
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeOnChangeResponse(JNIEnv* jenv, jobject thiz, jstring changeResponse)
{
	const char *nativeChangeResponse = jenv->GetStringUTFChars(changeResponse, 0);
	FString Response = FString(nativeChangeResponse);
	jenv->ReleaseStringUTFChars(changeResponse, nativeChangeResponse);
	UAndroidHapticLibrary::ParsePlayerResponse(Response);
}

#endif //  PLATFORM_ANDROID

void UAndroidHapticLibrary::UpdateDevices(TArray<FDevice> DeviceList)
{
	FoundDevices = DeviceList;
	UpdateDeviceListDelegate.ExecuteIfBound(DeviceList);
}

void UAndroidHapticLibrary::ParsePlayerResponse(FString ResponseString)
{
	UE_LOG(LogTemp, Log, TEXT("ParsingResponse"));

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

void UAndroidHapticLibrary::SubmitFrame(const FString & Key, FHapticFrame Frame)
{
	FSubmitRequest Request = FSubmitRequest(Key, "frame", Frame);
	SubmitRequestToPlayer(Request);
}

void UAndroidHapticLibrary::SubmitRequestToPlayer(FSubmitRequest Request)
{

	TSharedPtr<FJsonObject> SubmitObject = FJsonObjectConverter::UStructToJsonObject(Request);
	if (SubmitObject.IsValid())
	{
		TSharedPtr<FJsonObject> RequestObject = MakeShareable(new FJsonObject);
		TArray<TSharedPtr<FJsonValue>> RegisterValues;
		TArray<TSharedPtr<FJsonValue>> SubmitValues;
		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(SubmitObject));
		SubmitValues.Add(JsonValue);
		RequestObject->SetArrayField("Register", RegisterValues);
		RequestObject->SetArrayField("Submit", SubmitValues);
		FString RequestString;
		TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>> > Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&RequestString);
		if (FJsonSerializer::Serialize(RequestObject.ToSharedRef(), Writer))
		{
			AndroidThunkCpp_Submit(RequestString);
		}
	}

}

void UAndroidHapticLibrary::SubmitRequestToPlayer(FRegisterRequest Request)
{
	TSharedPtr<FJsonObject> RequestObject = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> RegisterValues;
	TArray<TSharedPtr<FJsonValue>> SubmitValues;
	TSharedPtr<FJsonObject> RegisterObject = MakeShareable(new FJsonObject);
	Request.ToJsonObject(*RegisterObject);
	TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(RegisterObject));
	RegisterValues.Add(JsonValue);
	RequestObject->SetArrayField("Register", RegisterValues);
	RequestObject->SetArrayField("Submit", SubmitValues);
	FString RequestString;
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>> > Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&RequestString);
	if (FJsonSerializer::Serialize(RequestObject.ToSharedRef(), Writer))
	{
		AndroidThunkCpp_Submit(RequestString);
	}
}

void UAndroidHapticLibrary::AndroidThunkCpp_Submit(FString PlayerSubmission)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID SubmitMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Submit", "(Ljava/lang/String;)V", false);
		jstring PlayerSubmissionJava = Env->NewStringUTF(TCHAR_TO_UTF8(*PlayerSubmission));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, SubmitMethod, PlayerSubmissionJava);
		Env->DeleteLocalRef(PlayerSubmissionJava);
	}
#endif // PLATFORM_ANDROID
}