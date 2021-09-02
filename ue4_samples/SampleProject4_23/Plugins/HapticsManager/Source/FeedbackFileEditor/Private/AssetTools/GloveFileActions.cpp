//Copyright bHaptics Inc. 2017-2019

#include "GloveFileActions.h"

#include "Feedback/TactotFeedbackFile.h"


UClass * FGloveFileActions::GetSupportedClass() const
{
	return UTactotFeedbackFile::StaticClass();
}
