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

#include "FeedbackFile.h"
#include "Feedback/TactotFeedbackFile.h"
#include "Feedback/TactosyFeedbackFile.h"
#include "Feedback/TactalFeedbackFile.h"
#include "Feedback/TactalFeedbackFile.h"

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
	FGuid Id = FGuid::NewGuid();
	FString ProjectString = "";
	FString Key = "";
	FString Device = "Tactot";
	float Duration = 0;

	if (FFileHelper::LoadFileToString(TextString, *Filename))
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TextString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString OutputString;
			TSharedPtr<FJsonObject> JsonProject = JsonObject->GetObjectField("project");
			TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutputString);
			FJsonSerializer::Serialize(JsonProject.ToSharedRef(), Writer);

			ProjectString = OutputString;
			Key = JsonObject->GetStringField("name");
			Device = JsonProject->GetObjectField("layout")->GetStringField("name");
			Duration = JsonProject->GetNumberField("mediaFileDuration");
		}
	}

	bOutOperationCanceled = false;

	if (Device == "Tactot")
	{
		UTactotFeedbackFile* TactotFile = nullptr;
		TactotFile = NewObject<UTactotFeedbackFile>(InParent, UTactotFeedbackFile::StaticClass(), InName, Flags);
		TactotFile->Id = Id;
		TactotFile->ProjectString = ProjectString;
		TactotFile->Key = Key;
		TactotFile->Device = Device;
		TactotFile->Duration = Duration;
		return TactotFile;
	}
	else if (Device == "Tactosy")
	{
		UTactosyFeedbackFile* TactosyFile = nullptr;
		TactosyFile = NewObject<UTactosyFeedbackFile>(InParent, UTactosyFeedbackFile::StaticClass(), InName, Flags);
		TactosyFile->Id = Id;
		TactosyFile->ProjectString = ProjectString;
		TactosyFile->Key = Key;
		TactosyFile->Device = Device;
		TactosyFile->Duration = Duration;
		return TactosyFile;
	}
	else if (Device == "Tactal")
	{
		UTactalFeedbackFile* TactalFile = nullptr;
		TactalFile = NewObject<UTactalFeedbackFile>(InParent, UTactalFeedbackFile::StaticClass(), InName, Flags);
		TactalFile->Id = Id;
		TactalFile->ProjectString = ProjectString;
		TactalFile->Key = Key;
		TactalFile->Device = Device;
		TactalFile->Duration = Duration;
		return TactalFile;
	}

	FeedbackFile = NewObject<UFeedbackFile>(InParent, InClass, InName, Flags);
	FeedbackFile->Id = Id;
	FeedbackFile->ProjectString = ProjectString;
	FeedbackFile->Key = Key;
	FeedbackFile->Device = Device;
	FeedbackFile->Duration = Duration;
	
	return FeedbackFile;
}