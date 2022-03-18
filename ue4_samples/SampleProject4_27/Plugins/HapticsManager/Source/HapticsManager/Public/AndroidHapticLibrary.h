// Copyright 2015-2022 bHaptics, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "HapticStructures.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif

#include "AndroidHapticLibrary.generated.h"

typedef TArray<FHapticFeedback> FDeviceStatus;

DECLARE_DELEGATE_OneParam(FDeviceArrayDelegate, TArray<FDevice>);
DECLARE_DELEGATE_OneParam(FDeviceStatusDelegate, FDeviceStatus);

UCLASS()
class UAndroidHapticLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

		//static void UpdateDevices(TArray<FDevice> DeviceList);

		UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentDevices", Keywords = "Haptics"), Category = "bHaptics(Android)")
		static TArray<FDevice> GetCurrentDevices();

	static void SubmitDot(
		FString Key, FString Pos, TArray<FDotPoint> Points, int DurationMillis);
	static void SubmitPath(
		FString Key, FString Pos, TArray<FPathPoint> Points, int DurationMillis);
	static void TurnOff(
		FString Key);
	static void TurnOffAll();

	static bool IsFeedbackRegistered(FString key);
	static bool IsFeedbackPlaying(FString key);
	static bool IsAnyFeedbackPlaying();
	static TArray<uint8> GetPositionStatus(FString pos);

	static void RegisterProject(FString key, FString fileStr);
	static void RegisterProjectReflected(FString key, FString fileStr);
	static bool AndroidThunkCpp_Initialize(FString appName);



	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsBhapticsDeviceConnceted", Keywords = "Haptics"), Category = "bHaptics(Android)")
		static bool IsDeviceConnceted(FString Position);

	static void SubmitRegistered(
		FString key, FString altKey, float intensity, float duration, float xOffsetAngle, float yOffset);

	//Ping Haptic Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PingHapticDevice", Keywords = "Haptics"), Category = "bHaptics(Android)")
		static void AndroidThunkCpp_Ping(FString DeviceAddress);
	//Ping Haptic Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PingByPosition", Keywords = "Haptics"), Category = "bHaptics(Android)")
		static void AndroidThunkJava_PingPosition(FString Position);

	static bool IsDeviceConnceted(EPosition Position);

	//Ping all Haptic Devices
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PingAll", Keywords = "Haptics"), Category = "bHaptics(Android)")
		static void AndroidThunkCpp_PingAll();

	//Set the Position of Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeDevicePosition", Keywords = "Haptics"), Category = "bHaptics(Android)")
		static void AndroidThunkCpp_ChangePosition(FString DeviceAddress, FString Position);

	//Toggle the Position of Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ToggleDevicePosition", Keywords = "Haptics"), Category = "bHaptics(Android)")
		static void AndroidThunkCpp_TogglePosition(FString DeviceAddress);


private:
	static FPlayerResponse CurrentResponse;
	static FPlayerResponse LastUpdatedResponse;
	static TArray<FDevice> DeviceList;

};
