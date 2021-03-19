// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

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

	static void UpdateDevices(TArray<FDevice> DeviceList);
	static void ParsePlayerResponse(FString ResponseString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentDevices", Keywords = "Haptics"), Category = "Haptics")
		static TArray<FDevice> GetCurrentDevices() { return FoundDevices; };

	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCurrentResponse", Keywords = "Haptics"), Category = "Haptics")
	static FPlayerResponse GetCurrentResponse();

	static FDeviceStatusDelegate UpdateDeviceStatusDelegate;
	
	static FDeviceArrayDelegate UpdateDeviceListDelegate;

	static void SubmitDot(
		FString Key, FString Pos, TArray<FDotPoint> Points, int DurationMillis);
	static void SubmitPath(
		FString Key, FString Pos, TArray<FPathPoint> Points, int DurationMillis);
	static void TurnOff(
		FString Key);	
	static void TurnOffAll();

	static bool IsDeviceConnceted(EPosition Position);

	static bool IsFeedbackRegistered(FString key);
	static bool IsFeedbackPlaying(FString key);
	static bool IsAnyFeedbackPlaying();
	static TArray<uint8> GetPositionStatus(FString pos);

	static void RegisterProject(FString key, FString fileStr);
	static void RegisterProjectReflected(FString key, FString fileStr);

	static void SubmitRegistered(
		FString key, FString altKey, float intensity, float duration, float xOffsetAngle, float yOffset);

	//Scan for bHaptics devices
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartHapticDeviceScan", Keywords = "Haptics"), Category = "Haptics")
		static void AndroidThunkCpp_StartScanning();

	//Stop scanning for devices
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopHapticDeviceScan", Keywords = "Haptics"), Category = "Haptics")
		static void AndroidThunkCpp_StopScanning();

	//Check if its currently scanning
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsHapticDeviceScanning", Keywords = "Haptics"), Category = "Haptics")
		static bool AndroidThunkCpp_IsScanning();

	//Pair with Haptic Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PairHapticDevice", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_Pair(FString DeviceAddress);

	//Pair with Haptic Device and set Position
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PairHapticDevicePosition", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_PairFromPosition(FString DeviceAddress, FString DevicePosition);

	//Unpair Haptic Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnpairHapticDevice", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_Unpair(FString DeviceAddress);
	
	//Unpair all Haptic Devices
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnpairAll", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_UnpairAll();
	
	//Ping Haptic Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PingHapticDevice", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_Ping(FString DeviceAddress);
	
	//Ping all Haptic Devices
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PingAll", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_PingAll();

	//Set the Position of Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeDevicePosition", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_ChangePosition(FString DeviceAddress, FString Position);
	
	//Toggle the Position of Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ToggleDevicePosition", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_TogglePosition(FString DeviceAddress);

	// Streaming Functions
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartHapticStreaming (Android)", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_StartStreaming(bool autoConnect);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopHapticStreaming (Android)", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_StopStreaming();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetHapticStreamingHosts (Android)", Keywords = "Haptics"), Category = "Haptics")
	static TArray<FHapticStreamingDevice> AndroidThunkCpp_GetStreamingHosts();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RefreshHapticStreamingHosts (Android)", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_RefreshStreamingHosts();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ConnectHapticStreamingHosts (Android)", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_ConnectStreamingHosts(FString host);
private:
	static TArray<FDevice> FoundDevices;
	static FPlayerResponse CurrentResponse;
	static FPlayerResponse LastUpdatedResponse;
	static EPosition StringToPosition(FString PositionString);
	static FCriticalSection m_Mutex;


	static TArray<FHapticStreamingDevice> Parse(FString JsonString);

};
