// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class FFeedbackFileEditorModule : public IModuleInterface
{
public:

	TSharedPtr<FSlateStyleSet> StyleSet;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};