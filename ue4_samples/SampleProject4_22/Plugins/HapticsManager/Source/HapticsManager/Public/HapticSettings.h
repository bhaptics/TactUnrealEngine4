// Copyright bHaptics Inc. 2017-2019

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DeveloperSettings.h"
#include "HapticSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Haptic Settings"))
class HAPTICSMANAGER_API UHapticSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()
	
public:
	

	// Variable to control if the Player should launch when starting the level, launching by default.
	// If true, bHaptics Player will launch when the game is run, if its installed.
	// If false, the user must launch the Player themselves.
	UPROPERTY(EditAnywhere, config, Category = Haptic)
		bool bShouldLaunch = true;

};
