//Copyright bHaptics Inc. 2017-2019

#include "HandFileActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Feedback/HandFeedbackFile.h"
#include "Styling/SlateStyle.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

FHandFileActions::FHandFileActions(const TSharedRef<FSlateStyleSet>& InStyle)
	: Style(InStyle)
{
}

uint32 FHandFileActions::GetCategories()
{
	return EAssetTypeCategories::None;
}

FText FHandFileActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_FeedbackFile", "Feedback File");
}

UClass * FHandFileActions::GetSupportedClass() const
{
	return UHandFeedbackFile::StaticClass();
}

FColor FHandFileActions::GetTypeColor() const
{
	return FColor::Yellow;
}

bool FHandFileActions::CanFilter()
{
	return true;
}

void FHandFileActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder & MenuBuilder)
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

bool FHandFileActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

#undef LOCTEXT_NAMESPACE
