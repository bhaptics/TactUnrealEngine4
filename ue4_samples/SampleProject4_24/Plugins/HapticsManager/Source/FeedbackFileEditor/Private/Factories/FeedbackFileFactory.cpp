//Copyright bHaptics Inc. 2017-2019

#include "FeedbackFileFactory.h"
#include "Containers/UnrealString.h"
#include "FeedbackFile.h"
#include "Misc/FileHelper.h"

#include "Templates/SharedPointer.h"

#include "Json/Public/Dom/JsonObject.h"
#include "Json/Public/Serialization/JsonReader.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "Json/Public/Serialization/JsonWriter.h"
#include "Json/Public/Policies/CondensedJsonPrintPolicy.h"

#include "Feedback/TactotFeedbackFile.h"
#include "Feedback/TactosyFeedbackFile.h"
#include "Feedback/TactalFeedbackFile.h"
#include "Feedback/HandFeedbackFile.h"
#include "Feedback/FootFeedbackFile.h"

#include "EditorFramework/AssetImportData.h"

UFeedbackFileFactory::UFeedbackFileFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Formats.Add(FString(TEXT("tact;")) + NSLOCTEXT("UFeedbackFileFactory", "FormatTact", "Tact File").ToString());
	SupportedClass = UFeedbackFile::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
	Formats.Add(TEXT("tact;bHaptics Feedback"));
}

UObject* UFeedbackFileFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{

	TSharedPtr<FJsonObject> JsonProject = LoadFileToJson(Filename);
	if (!JsonProject.IsValid())
		return nullptr;

	FString ProjectString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&ProjectString);
	FJsonSerializer::Serialize(JsonProject.ToSharedRef(), Writer);

	FString Key = JsonProject->GetStringField("name");
	FString Device = JsonProject->GetObjectField("layout")->GetStringField("type");
	float Duration = JsonProject->GetNumberField("mediaFileDuration");

	UFeedbackFile* FeedbackFile = UFeedbackFileFactory::CreateFeebackFile(Device, InParent, InName, Flags);

	FeedbackFile->Id = FGuid::NewGuid();
	FeedbackFile->ProjectString = ProjectString;
	FeedbackFile->Key = Key;
	FeedbackFile->Device = Device;
	FeedbackFile->Duration = Duration;

	FeedbackFile->AssetImportData->Update(Filename);

	bOutOperationCanceled = false;
	
	return FeedbackFile;
}



UFeedbackFile* UFeedbackFileFactory::CreateFeebackFile(const FString& Device, UObject* InParent, FName InName, EObjectFlags Flags)
{
	if (Device.StartsWith("Vest") || Device.StartsWith("Tactot"))
	{
		return NewObject<UTactotFeedbackFile>(InParent, UTactotFeedbackFile::StaticClass(), InName, Flags);
	}
	else if (Device.StartsWith("Tactosy"))
	{
		return NewObject<UTactosyFeedbackFile>(InParent, UTactosyFeedbackFile::StaticClass(), InName, Flags);
	}
	else if (Device.StartsWith("Tactal") || Device.StartsWith("Head"))
	{
		return NewObject<UTactalFeedbackFile>(InParent, UTactalFeedbackFile::StaticClass(), InName, Flags);
	}
	else if (Device.StartsWith("Hand"))
	{
		return NewObject<UHandFeedbackFile>(InParent, UHandFeedbackFile::StaticClass(), InName, Flags);
	}
	else if (Device.StartsWith("Foot"))
	{
		return NewObject<UFootFeedbackFile>(InParent, UFootFeedbackFile::StaticClass(), InName, Flags);
	}

	return NewObject<UFeedbackFile>(InParent, UFootFeedbackFile::StaticClass(), InName, Flags);
}




TSharedPtr<FJsonObject> UFeedbackFileFactory::LoadFileToJson(const FString& Filename)
{
	FString TextString;

	if (FFileHelper::LoadFileToString(TextString, *Filename))
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TextString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TSharedPtr<FJsonObject> JsonProject = JsonObject->GetObjectField("project");
			return JsonProject;
		}
	}

	return TSharedPtr<FJsonObject>();
}

