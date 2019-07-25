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
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Initialise", Keywords = "Haptics"), Category = "Haptics")
		void Initialise(FDevice LinkedDevice);

	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pair", Keywords = "Haptics"), Category = "Haptics")
		void Pair();
	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Unpair", Keywords = "Haptics"), Category = "Haptics")
		void Unpair();

	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ping", Keywords = "Haptics"), Category = "Haptics")
		void Ping();

	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangePosition", Keywords = "Haptics"), Category = "Haptics")
		void ChangePosition(FString Position);
	//BP
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TogglePosition", Keywords = "Haptics"), Category = "Haptics")
		void TogglePosition();

private:
	bool IsInitialised;
};
