//Copyright bHaptics Inc. 2017

#if(ENGINE_MINOR_VERSION < 16)
//#include "FeedbackFileEditor.h"
#endif

#include "FeedbackFileFactoryNew.h"
#include "FeedbackFile.h"

UFeedbackFileFactoryNew::UFeedbackFileFactoryNew(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UFeedbackFile::StaticClass();
	bCreateNew = false;
	bEditAfterNew = false;
}

/* UFactory overrides
*****************************************************************************/

UObject* UFeedbackFileFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UFeedbackFile>(InParent, InClass, InName, Flags);
}


bool UFeedbackFileFactoryNew::ShouldShowInNewMenu() const
{
	return false;
}