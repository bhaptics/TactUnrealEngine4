//Copyright bHaptics Inc. 2017-2019

#include "HapticsManagerActor.h"
#include "HapticStructures.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AndroidHapticLibrary.h"
#include "BhapticsLibrary.h"


// Sets default values
AHapticsManagerActor::AHapticsManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHapticsManagerActor::BeginDestroy()
{
	Super::BeginDestroy();
	UAndroidHapticLibrary::AndroidThunkCpp_TurnOffVisualization();
}

// Called when the game starts or when spawned
void AHapticsManagerActor::BeginPlay()
{
	Super::BeginPlay();

	InitialiseDots(Tactal);
	InitialiseDots(TactosyLeft);
	InitialiseDots(TactosyRight);
	InitialiseDots(TactotBack);
	InitialiseDots(TactotFront);
	InitialiseDots(TactGloveLeft);
	InitialiseDots(TactGloveRight);
	InitialiseDots(TactShoeLeft);
	InitialiseDots(TactShoeRight);

	UAndroidHapticLibrary::AndroidThunkCpp_TurnOnVisualization();
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

	TArray<FHapticFeedback> Visualisation = BhapticsLibrary::Lib_GetResponseStatus();

	for (int i = 0; i < Visualisation.Num(); i++)
	{
		FHapticFeedback Feedback = Visualisation[i];

		switch (Feedback.Position)
		{
		case EPosition::ForearmR:
			VisualiseFeedback(Feedback, TactosyRight);
			break;
		case EPosition::ForearmL:
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
		default:
			printf("Position not found.");
			break;
		}
	}

	IsTicking = false;
}

void AHapticsManagerActor::InitialiseDots(TArray<USceneComponent*> TactSuitItem, float Scale)
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
		DotMesh->SetRelativeScale3D(FVector(0.3, 0.3*Scale, 0.5*Scale));
	}
}

void AHapticsManagerActor::VisualiseFeedback(FHapticFeedback Feedback, TArray<USceneComponent*> TactSuitItem, float DeviceScale)
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
	USceneComponent * VestBack, USceneComponent * GloveLeft, USceneComponent * GloveRight, USceneComponent * ShoeLeft, USceneComponent * ShoeRight)
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

}