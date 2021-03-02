//Copyright bHaptics Inc. 2017-2019

#pragma once

#include "CoreTypes.h"
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"
#include "EditorReimportHandler.h"
#include "FeedbackFileFactory.generated.h"

class UFeedbackFile;

UCLASS(hidecategories = Object)
class UFeedbackFileFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	
protected:
	static UFeedbackFile* CreateFeebackFile(const FString& Device, UObject* InParent, FName InName, EObjectFlags Flags);
	static TSharedPtr<FJsonObject> LoadFileToJson(const FString& Filename);
};
