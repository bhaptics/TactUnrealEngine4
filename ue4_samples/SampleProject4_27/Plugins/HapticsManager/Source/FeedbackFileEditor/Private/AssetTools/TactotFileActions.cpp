//Copyright bHaptics Inc. 2017-2019

#include "TactotFileActions.h"

#include "Feedback/TactotFeedbackFile.h"


UClass * FTactotFileActions::GetSupportedClass() const
{
	return UTactotFeedbackFile::StaticClass();
}
