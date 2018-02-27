// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Developer/AssetTools/Public/AssetTypeActions_Base.h"
#include "Templates/SharedPointer.h"

class FSlateStyleSet;
/**
 * 
 */
class FFeedbackFileActions
	: public FAssetTypeActions_Base
{
public:
	FFeedbackFileActions(const TSharedRef<FSlateStyleSet>& InStyle);

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
