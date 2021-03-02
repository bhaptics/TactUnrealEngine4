//Copyright bHaptics Inc. 2017-2019

#include "FootFileActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Feedback/FootFeedbackFile.h"
#include "Styling/SlateStyle.h"

UClass * FFootFileActions::GetSupportedClass() const
{
	return UFootFeedbackFile::StaticClass();
}

