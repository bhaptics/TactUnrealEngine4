//Copyright bHaptics Inc. 2018


#include "TactalFileActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Feedback/TactalFeedbackFile.h"
#include "Styling/SlateStyle.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

FTactalFileActions::FTactalFileActions(const TSharedRef<FSlateStyleSet>& InStyle)
	: Style(InStyle)
{
}

uint32 FTactalFileActions::GetCategories()
{
	return EAssetTypeCategories::None;
}

FText FTactalFileActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_FeedbackFile", "Feedback File");
}

UClass * FTactalFileActions::GetSupportedClass() const
{
	return UTactalFeedbackFile::StaticClass();
}

FColor FTactalFileActions::GetTypeColor() const
{
	//return FColor::Cyan;
	return FColor::Purple;
}

bool FTactalFileActions::CanFilter()
{
	return true;
}

void FTactalFileActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder & MenuBuilder)
{
	//FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	//auto FeedbackFiles = GetTypedWeakObjectPtrs<UFeedbackFile>(InObjects);

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

bool FTactalFileActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

#undef LOCTEXT_NAMESPACE
