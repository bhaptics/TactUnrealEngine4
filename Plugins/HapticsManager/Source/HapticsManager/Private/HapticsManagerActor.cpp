//Copyright bHaptics Inc. 2018

#include "HapticsManager.h"
#include "HapticsManagerActor.h"
#include "HapticStructures.h"
#include "BhapticsLibrary.h"
#include "BhapticsUtilities.h"

FCriticalSection AHapticsManagerActor::m_Mutex;

// Sets default values
AHapticsManagerActor::AHapticsManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FGuid Gui = FGuid::NewGuid();
	Id = Gui.ToString();
}

void AHapticsManagerActor::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void AHapticsManagerActor::BeginPlay()
{
	Super::BeginPlay();
	ChangedFeedbacks = {};

	//bhaptics::HapticPlayer::instance()->registerConnection(StandardId);
	//RegisterConnection(StandardId);

	InitialiseDots(Tactal);
	InitialiseDots(TactosyLeft);
	InitialiseDots(TactosyRight);
	InitialiseDots(TactotBack);
	InitialiseDots(TactotFront);
	InitialiseDots(TactGloveLeft);
	InitialiseDots(TactGloveRight);
	InitialiseDots(TactShoeLeft);
	InitialiseDots(TactShoeRight);
	InitialiseDots(TactRacket);

}

// Called every frame
void AHapticsManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTicking)
	{
		return;
	}

	IsTicking = true;

	UpdateFeedback();
	TArray<FHapticFeedback> Visualisation = ChangedFeedbacks;

	for (int i = 0; i < Visualisation.Num(); i++)
	{
		FHapticFeedback Feedback = Visualisation[i];

		switch (Feedback.Position)
		{
		case EPosition::Right:
			VisualiseFeedback(Feedback, TactosyRight);
			break;
		case EPosition::Left:
			VisualiseFeedback(Feedback, TactosyLeft);
			break;
		case EPosition::VestFront:
			VisualiseFeedback(Feedback, TactotFront);
			break;
		case EPosition::VestBack:
			VisualiseFeedback(Feedback, TactotBack);
			break;
		case EPosition::Head:
			VisualiseFeedback(Feedback, Tactal);
			break;
		case EPosition::HandL:
			VisualiseFeedback(Feedback, TactGloveLeft);
			break;
		case EPosition::HandR:
			VisualiseFeedback(Feedback, TactGloveRight);
			break;
		case EPosition::FootL:
			VisualiseFeedback(Feedback, TactShoeLeft);
			break;
		case EPosition::FootR:
			VisualiseFeedback(Feedback, TactShoeRight);
			break;
		case EPosition::Racket:
			VisualiseFeedback(Feedback, TactRacket);
		default:
			printf("Position not found.");
			break;
		}
	}

	ChangedFeedbacks.Empty();

	IsTicking = false;
}

void AHapticsManagerActor::UpdateFeedback()
{
	//std::map<std::string, std::vector<int>> DeviceMotors = bhaptics::HapticPlayer::instance()->getResponseStatus();//fix
	ChangedFeedbacks = BhapticsLibrary::Lib_GetResponseStatus();
}

void AHapticsManagerActor::InitialiseDots(TArray<USceneComponent*> TactSuitItem)
{
	for (int i = 0; i < TactSuitItem.Num(); i++)
	{
		UStaticMeshComponent* DotMesh = Cast<UStaticMeshComponent>(TactSuitItem[i]);
		if (DotMesh == NULL)
		{
			continue;
		}

		UMaterialInstanceDynamic* DotMaterial = DotMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, DotMesh->GetMaterial(0));
		DotMaterial->SetVectorParameterValue("Base Color", FLinearColor(0.8, 0.8, 0.8, 1.0));
		DotMesh->SetRelativeScale3D(FVector(0.3, 0.3, 0.5));
	}
}

void AHapticsManagerActor::VisualiseFeedback(FHapticFeedback Feedback, TArray<USceneComponent*> TactSuitItem)
{
	for (int i = 0; i < TactSuitItem.Num(); i++)
	{
		FString ComponentName = TactSuitItem[i]->GetName();
		ComponentName.RemoveAt(0, 2);
		float DotPosition = FCString::Atof(*ComponentName);
		float Scale = Feedback.Values[DotPosition] / 100.0;
		UStaticMeshComponent* DotMesh = Cast<UStaticMeshComponent>(TactSuitItem[i]);

		if (DotMesh == NULL)
		{
			continue;
		}

		UMaterialInstanceDynamic* DotMaterial = Cast<UMaterialInstanceDynamic>(DotMesh->GetMaterial(0));
		DotMaterial->SetVectorParameterValue("Base Color", FLinearColor(0.8 + Scale * 0.2, 0.8 + Scale * 0.01, 0.8 - Scale * 0.8, 1.0));
		DotMesh->SetRelativeScale3D(FVector(0.3 + 0.15*(Scale*0.8), 0.3 + 0.15*(Scale*0.8), 0.5 + 0.15*(Scale*0.8)));
	}
}

void AHapticsManagerActor::SetTactSuit(USceneComponent * SleeveLeft, USceneComponent * SleeveRight, USceneComponent * Head, USceneComponent * VestFront,
	USceneComponent * VestBack, USceneComponent * GloveLeft, USceneComponent * GloveRight, USceneComponent * ShoeLeft, USceneComponent * ShoeRight, USceneComponent * Racket)
{
	if (SleeveLeft != NULL)
	{
		SleeveLeft->GetChildrenComponents(false, TactosyLeft);
	}

	if (SleeveRight != NULL)
	{
		SleeveRight->GetChildrenComponents(false, TactosyRight);
	}

	if (Head != NULL)
	{
		Head->GetChildrenComponents(false, Tactal);
	}
	if (VestFront != NULL)
	{
		VestFront->GetChildrenComponents(false, TactotFront);
	}
	if (VestBack != NULL)
	{
		VestBack->GetChildrenComponents(false, TactotBack);
	}

	if (GloveLeft != NULL)
	{
		GloveLeft->GetChildrenComponents(false, TactGloveLeft);
	}

	if (GloveRight != NULL)
	{
		GloveRight->GetChildrenComponents(false, TactGloveRight);
	}

	if (ShoeLeft != NULL)
	{
		ShoeLeft->GetChildrenComponents(false, TactShoeLeft);
	}

	if (ShoeRight != NULL)
	{
		ShoeRight->GetChildrenComponents(false, TactShoeRight);
	}

	if (Racket != NULL)
	{
		Racket->GetChildrenComponents(false, TactRacket);
	}
}