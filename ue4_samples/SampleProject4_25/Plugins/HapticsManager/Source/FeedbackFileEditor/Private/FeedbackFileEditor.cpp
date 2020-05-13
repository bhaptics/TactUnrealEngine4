// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FeedbackFileEditor.h"

#include "Interfaces/IPluginManager.h"

#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyle.h"

#include "Templates/SharedPointer.h"
#include "AssetTools/FeedbackFileActions.h"
#include "AssetTools/TactosyFileActions.h"
#include "AssetTools/TactotFileActions.h"
#include "AssetTools/TactalFileActions.h"
#include "AssetTools/HandFileActions.h"
#include "AssetTools/FootFileActions.h"

TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

#define LOCTEXT_NAMESPACE "FFeedbackFileEditorModule"

void FFeedbackFileEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	StyleSet = MakeShareable(new FSlateStyleSet("FeedbackFileStyle"));

	//Content path of this plugin
	FString ContentDir = IPluginManager::Get().FindPlugin("HapticsManager")->GetBaseDir();

	//The image we wish to load is located inside the Resources folder inside the Base Directory
	//so let's set the content dir to the base dir and manually switch to the Resources folder:
	StyleSet->SetContentRoot(ContentDir);

	//Create a brush from the icon
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/icon128"), TEXT(".png")), FVector2D(128.f, 128.f));
	FSlateImageBrush* ThumbnailBrushArm = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/icon_tactosy600"), TEXT(".png")), FVector2D(256.f, 256.f));
	FSlateImageBrush* ThumbnailBrushHead = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/icon_tactal600"), TEXT(".png")), FVector2D(256.f, 256.f));
	FSlateImageBrush* ThumbnailBrushBody = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/icon_tactot600"), TEXT(".png")), FVector2D(256.f, 256.f));
	FSlateImageBrush* ThumbnailBrushHand = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/icon_hand600"), TEXT(".png")), FVector2D(256.f, 256.f));
	FSlateImageBrush* ThumbnailBrushFoot = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/icon_Foot600"), TEXT(".png")), FVector2D(256.f, 256.f));

	if (ThumbnailBrush)
	{
		//In order to bind the thumbnail to our class we need to type ClassThumbnail.X where X is the name of the C++ class of the asset
		StyleSet->Set("ClassThumbnail.FeedbackFile", ThumbnailBrush);
	}

	if (ThumbnailBrushBody)
	{
		StyleSet->Set("ClassThumbnail.TactotFeedbackFile", ThumbnailBrushBody);
	}

	if (ThumbnailBrushArm)
	{
		StyleSet->Set("ClassThumbnail.TactosyFeedbackFile", ThumbnailBrushArm);
	}

	if (ThumbnailBrushHead)
	{
		StyleSet->Set("ClassThumbnail.TactalFeedbackFile", ThumbnailBrushHead);
	}

	if (ThumbnailBrushHand)
	{
		StyleSet->Set("ClassThumbnail.HandFeedbackFile", ThumbnailBrushHand);
	}

	if (ThumbnailBrushFoot)
	{
		StyleSet->Set("ClassThumbnail.FootFeedbackFile", ThumbnailBrushFoot);
	}

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	auto Action = MakeShareable(new FFeedbackFileActions(StyleSet.ToSharedRef()));
	auto ActionVest = MakeShareable(new FTactotFileActions(StyleSet.ToSharedRef()));
	auto ActionArm = MakeShareable(new FTactosyFileActions(StyleSet.ToSharedRef()));
	auto ActionHead = MakeShareable(new FTactalFileActions(StyleSet.ToSharedRef()));
	auto ActionHand = MakeShareable(new FHandFileActions(StyleSet.ToSharedRef()));
	auto ActionFoot = MakeShareable(new FFootFileActions(StyleSet.ToSharedRef()));

	AssetTools.RegisterAssetTypeActions(Action);
	AssetTools.RegisterAssetTypeActions(ActionVest);
	AssetTools.RegisterAssetTypeActions(ActionArm);
	AssetTools.RegisterAssetTypeActions(ActionHead);
	AssetTools.RegisterAssetTypeActions(ActionHand);
	AssetTools.RegisterAssetTypeActions(ActionFoot);

	RegisteredAssetTypeActions.Add(Action);
	RegisteredAssetTypeActions.Add(ActionVest);
	RegisteredAssetTypeActions.Add(ActionArm);
	RegisteredAssetTypeActions.Add(ActionHead);
	RegisteredAssetTypeActions.Add(ActionHand);
	RegisteredAssetTypeActions.Add(ActionFoot);

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}

void FFeedbackFileEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());
	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");

	if (AssetToolsModule != nullptr)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();

		for (auto Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFeedbackFileEditorModule, FeedbackFileEditor)