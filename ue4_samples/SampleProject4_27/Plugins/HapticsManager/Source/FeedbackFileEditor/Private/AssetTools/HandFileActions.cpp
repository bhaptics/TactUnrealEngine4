//Copyright bHaptics Inc. 2017-2019

#include "HandFileActions.h"

#include "Feedback/HandFeedbackFile.h"

UClass * FHandFileActions::GetSupportedClass() const
{
	return UHandFeedbackFile::StaticClass();
}
