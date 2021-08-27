//Copyright bHaptics Inc. 2017-2019

#include "FeedbackFileActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "FeedbackFile.h"
#include "Styling/SlateStyle.h"
#include "EditorFramework/AssetImportData.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"


bool FFeedbackFileActions::CanFilter()
{
	return true;
}

void FFeedbackFileActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder & MenuBuilder)
{
	auto FeedbackFiles = GetTypedWeakObjectPtrs<UFeedbackFile>(InObjects);
						
	//MenuBuilder.AddMenuEntry(
	//	LOCTEXT("FeedbackFile_PlayFeedback", "Play Feedback"),
	//	LOCTEXT("FeedbackFile_PlayFeedbackToolTip", "Play the selected feedback. To display on UI, use the HapticsManager_BP."),
	//	FSlateIcon(),
	//	FUIAction(
	//		FExecuteAction::CreateLambda([=] {
						
	//	for (auto& FeedbackFile : FeedbackFiles)
	//	{
	//		if (FeedbackFile.IsValid())
	//		{
						
	//			std::string StandardKey(TCHAR_TO_UTF8(*FeedbackFile->Key));
	//			if (!bhaptics::HapticPlayer::instance()->isFeedbackRegistered(StandardKey))
	//			{
	//				TSharedPtr<FJsonObject> JsonProject = MakeShareable(new FJsonObject);
	//				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FeedbackFile->ProjectString);
	//				if (FJsonSerializer::Deserialize(Reader, JsonProject))
	//				{
	//					bhaptics::HapticPlayer::instance()->registerFeedback(StandardKey, JsonProject);
	//				}
	//			}
	//			bhaptics::HapticPlayer::instance()->submitRegistered(StandardKey);
	//		}
	//	}
	//	//bhaptics::HapticPlayer::instance()->destroy();
						
	//}),
	//		FCanExecuteAction::CreateLambda([=] {
	//	for (auto& FeedbackFile : FeedbackFiles)
	//	{
	//		if (FeedbackFile.IsValid())
	//		{
	//			return true;
	//		}
	//	}
	//	return false;
	//})
	//	)
	//);
}

uint32 FFeedbackFileActions::GetCategories()
{
	return EAssetTypeCategories::None;
}

FText FFeedbackFileActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_FeedbackFile", "Feedback File");
}

UClass * FFeedbackFileActions::GetSupportedClass() const
{
	return UFeedbackFile::StaticClass();
}

FColor FFeedbackFileActions::GetTypeColor() const
{
	return FColor::Yellow;
}

bool FFeedbackFileActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

void FFeedbackFileActions::GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{
	for (auto& Asset : TypeAssets)
	{
		UFeedbackFile* Feedback = CastChecked<UFeedbackFile>(Asset);
		Feedback->AssetImportData->ExtractFilenames(OutSourceFilePaths);
	}
}

#undef LOCTEXT_NAMESPACE
