// Fill out your copyright notice in the Description page of Project Settings.

#include "FeedbackFileEditor.h"

#include "FeedbackFileFactory.h"
#include "Containers/UnrealString.h"
#include "FeedbackFile.h"
#include "Misc/FileHelper.h"

UFeedbackFileFactory::UFeedbackFileFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Formats.Add(FString(TEXT("tact;")) + NSLOCTEXT("UFeedbackFileFactory", "FormatTact", "Tact File").ToString());
	SupportedClass = UFeedbackFile::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}

UObject* UFeedbackFileFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UFeedbackFile* FeedbackFile = nullptr;
	FString TextString;

	if (FFileHelper::LoadFileToString(TextString, *Filename))
	{
		FeedbackFile = NewObject<UFeedbackFile>(InParent, InClass, InName, Flags);
		FeedbackFile->Key = Filename;
		FeedbackFile->Id = FGuid::NewGuid();

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TextString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString OutputString;
			TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutputString);
			FJsonSerializer::Serialize(JsonObject->GetObjectField("project").ToSharedRef(), Writer);

			FeedbackFile->Project = *JsonObject->GetObjectField("project");
			FeedbackFile->ProjectString = OutputString;
			//FeedbackFile->Proj.from_json(*JsonObject->GetObjectField("project"));
			FeedbackFile->Key = JsonObject->GetStringField("name");
		}
	}

	bOutOperationCanceled = false;

	return FeedbackFile;
}