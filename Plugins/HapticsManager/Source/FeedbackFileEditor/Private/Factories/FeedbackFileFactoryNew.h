//Copyright bHaptics Inc. 2017-2019

#pragma once

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"

#include "FeedbackFileFactoryNew.generated.h"

/**
 * 
 */
UCLASS(hidecategories = Object)
class UFeedbackFileFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override;
};
