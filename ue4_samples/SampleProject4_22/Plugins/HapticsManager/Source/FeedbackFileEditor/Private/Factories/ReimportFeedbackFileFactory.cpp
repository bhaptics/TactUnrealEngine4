//Copyright bHaptics Inc. 2017-2019

#include "ReimportFeedbackFileFactory.h"
#include "FeedbackFile.h"
#include "EditorFramework/AssetImportData.h"

UReimportFeedbackFileFactory::UReimportFeedbackFileFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UReimportFeedbackFileFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UFeedbackFile* Feedback = Cast<UFeedbackFile>(Obj);
	if (Feedback)
	{
		Feedback->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}
	return false;
}

void UReimportFeedbackFileFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UFeedbackFile* Feedback = Cast<UFeedbackFile>(Obj);
	if (Feedback && ensure(NewReimportPaths.Num() == 1))
	{
		Feedback->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

EReimportResult::Type UReimportFeedbackFileFactory::Reimport(UObject* Obj)
{
	UFeedbackFile* FeedbackFile = Cast<UFeedbackFile>(Obj);
	if (FeedbackFile == nullptr)
		return EReimportResult::Failed;

	TSharedPtr<FJsonObject> JsonProject = LoadFileToJson(FeedbackFile->AssetImportData->GetFirstFilename());
	if (!JsonProject.IsValid())
		return EReimportResult::Failed;

	FString ProjectString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&ProjectString);
	FJsonSerializer::Serialize(JsonProject.ToSharedRef(), Writer);

	FString Key = JsonProject->GetStringField("name");
	FString Device = JsonProject->GetObjectField("layout")->GetStringField("type");
	float Duration = JsonProject->GetNumberField("mediaFileDuration");

	if (Device != FeedbackFile->Device)
		return EReimportResult::Failed;

	FeedbackFile->ProjectString = ProjectString;
	FeedbackFile->Key = Key;
	FeedbackFile->Duration = Duration;

	FeedbackFile->MarkPackageDirty();

	return EReimportResult::Succeeded;
}

int32 UReimportFeedbackFileFactory::GetPriority() const
{
	return ImportPriority;
}
