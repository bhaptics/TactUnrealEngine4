//Copyright bHaptics Inc. 2018

#pragma once
#include "Developer/AssetTools/Public/AssetTypeActions_Base.h"
#include "Templates/SharedPointer.h"

/**
 * 
 */
class FFeedbackFileActions
	: public FAssetTypeActions_Base
{
public:

	virtual bool CanFilter() override;
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual uint32 GetCategories() override;
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;

	virtual void GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const override;
	virtual bool IsImportedAsset() const override { return true; }
};
