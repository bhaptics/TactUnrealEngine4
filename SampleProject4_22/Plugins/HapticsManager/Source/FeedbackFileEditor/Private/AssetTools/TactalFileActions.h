//Copyright bHaptics Inc. 2018

#pragma once
#include "Developer/AssetTools/Public/AssetTypeActions_Base.h"
#include "Templates/SharedPointer.h"
#include "FeedbackFileActions.h"

class FSlateStyleSet;
/**
 * 
 */
class FTactalFileActions
	: public FAssetTypeActions_Base
{
public:
	FTactalFileActions(const TSharedRef<FSlateStyleSet>& InStyle);
	
public:
	virtual bool CanFilter() override;
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual uint32 GetCategories() override;
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;

private:

	/** Pointer to the style set to use for toolkits. */
	TSharedRef<FSlateStyleSet> Style;
};
