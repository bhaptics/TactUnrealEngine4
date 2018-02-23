// Fill out your copyright notice in the Description page of Project Settings.

#include "FeedbackFileEditor.h"
#include "TactClipFactoryNew.h"
#include "TactClip.h"

UTactClipFactoryNew::UTactClipFactoryNew(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UTactClip::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

/* UFactory overrides
*****************************************************************************/

UObject* UTactClipFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UTactClip>(InParent, InClass, InName, Flags);
}


bool UTactClipFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}