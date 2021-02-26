// Fill out your copyright notice in the Description page of Project Settings.


#include "FeedbackFileReimportFactory.h"
#include "FeedbackFile.h"

UFeedbackFileReimportFactory::UFeedbackFileReimportFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UFeedbackFile::StaticClass();
	bCreateNew = false;
}

bool UFeedbackFileReimportFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UFeedbackFile* FeedbackFile = Cast<UFeedbackFile>(Obj);
	if (FeedbackFile != nullptr)
	{
		return true;
	}
	return false;
}

void UFeedbackFileReimportFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	if (NewReimportPaths.Num() == 1) {
		ReimportPath = NewReimportPaths[0];
		return;
	}
}

EReimportResult::Type UFeedbackFileReimportFactory::Reimport(UObject* Obj)
{
	if (ReimportPath.IsEmpty()) {
		return EReimportResult::Failed;
	}
	UFeedbackFile* FeedbackFile = Cast<UFeedbackFile>(Obj);
	bool OutCanceled = false;

	if (ImportObject(
		FeedbackFile->GetClass(), FeedbackFile->GetOuter(), *FeedbackFile->GetName(), RF_Public | RF_Standalone, ReimportPath, nullptr, OutCanceled) != nullptr)
	{
		return EReimportResult::Succeeded;
	}
	
	return EReimportResult::Failed;
}

int32 UFeedbackFileReimportFactory::GetPriority() const
{
	return 0;
}
