//Copyright bHaptics Inc. 2018

#pragma once
#include "FeedbackFileActions.h"


class FTactalFileActions : public FFeedbackFileActions
{
	
public:
	virtual UClass* GetSupportedClass() const override;
};
