//Copyright bHaptics Inc. 2017 - 2019

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HapticStructures.h"
#include "HapticDevice.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HAPTICSMANAGER_API UHapticDevice : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Android|Haptic")
		FDevice DeviceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Android|Haptic")
		bool DidUpdateDevice = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = false), Category = "Android|Haptic")
		bool IsDarkMode = false;
	
	//Set the data of the Haptic Device to use
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetDeviceData", Keywords = "Haptics"), Category = "Haptics")
		void SetDeviceData(FDevice LinkedDevice);

	//Ping the Haptic Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ping", Keywords = "Haptics"), Category = "Haptics")
		void Ping();

	//Set the Position for the Device (ie. Left/Right)
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangePosition", Keywords = "Haptics"), Category = "Haptics")
		void ChangePosition(FString Position);
	//Toggle the Position of the Device
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TogglePosition", Keywords = "Haptics"), Category = "Haptics")
		void TogglePosition();

private:
	bool IsInitialised;
};
