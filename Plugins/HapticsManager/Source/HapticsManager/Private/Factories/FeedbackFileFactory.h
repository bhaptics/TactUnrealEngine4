// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"
#include "FeedbackFileFactory.generated.h"

/**
 * 
 */
UCLASS(hidecategories = Object)
class UFeedbackFileFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public :
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	
};
