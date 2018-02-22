// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HapticStructures.h"
#include "FeedbackFile.h"
#include "TactSourceComponent.generated.h"

UENUM()
enum class EPlaybackMode
{
	DOT_MODE = 0,
	PATH_MODE = 1,
	FILE_MODE = 2
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPTICSMANAGER_API UTactSourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	//TArray<FDotPoint> DotMotors;
	//
	//TArray<FPathPoint> Points;

	//UFeedbackFile Feedback;

	//EPlaybackMode Mode;

	//// Sets default values for this component's properties
	//UTactSourceComponent();

	//// Called when the game starts
	//virtual void BeginPlay() override;
	//
	//// Called every frame
	//virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//void Play();

		
};
