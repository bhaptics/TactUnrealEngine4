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

	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartScan", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_StartScanning();

	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopScan", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_StopScanning();

	//BP
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsScanning", Keywords = "Haptics"), Category = "Haptics")
	static bool AndroidThunkCpp_IsScanning();

	//Library
	static void AndroidThunkCpp_Submit(FString PlayerSubmission);

	static void SubmitFrame(const FString & Key, FHapticFrame Frame);
	static void SubmitRequestToPlayer(FSubmitRequest Request);
	static void SubmitRequestToPlayer(FRegisterRequest Request);

	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pair", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_Pair(FString DeviceAddress);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PairPosition", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_PairFromPosition(FString DeviceAddress, FString DevicePosition);
	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Unpair", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_Unpair(FString DeviceAddress);
	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnpairAll", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_UnpairAll();
	
	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ping", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_Ping(FString DeviceAddress);
	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PingAll", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_PingAll();

	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangePosition", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_ChangePosition(FString DeviceAddress, FString Position);
	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TogglePosition", Keywords = "Haptics"), Category = "Haptics")
	static void AndroidThunkCpp_TogglePosition(FString DeviceAddress);



private:
	static TArray<FDevice> FoundDevices;
	static FPlayerResponse CurrentResponse;
	static FPlayerResponse LastUpdatedResponse;
	static EPosition StringToPosition(FString PositionString);
	static FCriticalSection m_Mutex;

};
