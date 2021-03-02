//Copyright bHaptics Inc. 2017-2019

#pragma once

#include "UObject/ObjectMacros.h"
#include "FeedbackFileFactory.h"
#include "ReimportFeedbackFileFactory.generated.h"


UCLASS()
class UReimportFeedbackFileFactory : public UFeedbackFileFactory, public FReimportHandler
{
	GENERATED_UCLASS_BODY()

public:
	//~ Begin FReimportHandler Interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	virtual int32 GetPriority() const override;
	//~ End FReimportHandler Interface
};