//Copyright bHaptics Inc. 2018


#include "TactalFileActions.h"

#include "Feedback/TactalFeedbackFile.h"

UClass * FTactalFileActions::GetSupportedClass() const
{
	return UTactalFeedbackFile::StaticClass();
}
