//Copyright bHaptics Inc. 2017-2019

#include "FeedbackFileActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "FeedbackFile.h"
#include "Styling/SlateStyle.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"
FFeedbackFileActions::FFeedbackFileActions(const TSharedRef<FSlateStyleSet>& InStyle)
	: Style(InStyle)
{
}

bool FFeedbackFileActions::CanFilter()
{
	return true;
}

void FFeedbackFileActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder & MenuBuilder)
{

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

#undef LOCTEXT_NAMESPACE
