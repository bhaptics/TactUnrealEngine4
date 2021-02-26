//Copyright bHaptics Inc. 2017-2019

#include "TactosyFileActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Feedback/TactosyFeedbackFile.h"
#include "Styling/SlateStyle.h"

UClass * FTactosyFileActions::GetSupportedClass() const
{
	return UTactosyFeedbackFile::StaticClass();
}

