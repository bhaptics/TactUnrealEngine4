//Copyright bHaptics Inc. 2017-2019

#pragma once
#include "FeedbackFileActions.h"

class FTactotFileActions : public FFeedbackFileActions
{

public:
	virtual UClass* GetSupportedClass() const override;
};
