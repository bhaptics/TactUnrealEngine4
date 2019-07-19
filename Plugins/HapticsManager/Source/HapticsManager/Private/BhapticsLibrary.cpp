//Copyright bHaptics Inc. 2017 - 2019

#include "BhapticsLibrary.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"
#include "HapticPlayer.h"
#include "HapticsManager.h"

//bool BhapticsLibrary::IsInitialised = false;
bool BhapticsLibrary::IsLoaded = false;
//FProcHandle BhapticsLibrary::Handle;
//bool BhapticsLibrary::Success = false;
AbstractHapticPlayer* BhapticsLibrary::Player = nullptr;

BhapticsLibrary::BhapticsLibrary()
{

}


BhapticsLibrary::~BhapticsLibrary()
{

}

void BhapticsLibrary::SetLibraryLoaded()
{
	IsLoaded = true;
	Player = new HapticPlayer();
}

bool BhapticsLibrary::InitialiseConnection()
{
	if (!IsLoaded)
	{
		return false;
	}

	//Initialise();

	return Player->InitialiseConnection();
}

void BhapticsLibrary::Free()
{
	if (!IsLoaded)
	{
		return;
	}

	//Destroy();
	Player->DestroyConnection();
}

void BhapticsLibrary::Lib_RegisterFeedback(FString Key, FString ProjectJson)
{
	if (!IsLoaded)
	{
		return;
	}

	Player->RegisterFeedbackProject(Key, ProjectJson);

}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key)
{
	if (!IsLoaded)
	{
		return;
	}

	Player->SubmitRegisteredFeedback(Key);
}

void BhapticsLibrary::Lib_SubmitRegistered(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption)
{
	if (!IsLoaded)
	{
		return;
	}
	
	Player->SubmitRegisteredFeedback(Key, AltKey, ScaleOpt, RotOption);
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<uint8> MotorBytes, int DurationMillis)
{
	if (!IsLoaded)
	{
		return;
	}

	Player->SubmitFeedback(Key, Pos, MotorBytes, DurationMillis);
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<FDotPoint> Points, int DurationMillis)
{
	if (!IsLoaded)
	{
		return;
	}
	
	Player->SubmitFeedback(Key, Pos, Points, DurationMillis);
}

void BhapticsLibrary::Lib_Submit(FString Key, EPosition Pos, TArray<FPathPoint> Points, int DurationMillis)
{
	if (!IsLoaded)
	{
		return;
	}
	
	Player->SubmitFeedback(Key, Pos, Points, DurationMillis);
}

bool BhapticsLibrary::Lib_IsFeedbackRegistered(FString Key)
{
	if (!IsLoaded)
	{
		return false;
	}
	return Player->IsRegistered(Key);
}

bool BhapticsLibrary::Lib_IsPlaying()
{
	if (!IsLoaded)
	{
		return false;
	}

	return Player->IsPlaying();
}

bool BhapticsLibrary::Lib_IsPlaying(FString Key)
{
	if (!IsLoaded)
	{
		return false;
	}

	return Player->IsPlaying(Key);
}

void BhapticsLibrary::Lib_TurnOff()
{
	if (!IsLoaded)
	{
		return;
	}

	Player->TurnOff();
}

void BhapticsLibrary::Lib_TurnOff(FString Key)
{
	if (!IsLoaded)
	{
		return;
	}
	Player->TurnOff(Key);
}

void BhapticsLibrary::Lib_EnableFeedback()
{
	if (!IsLoaded)
	{
		return;
	}
	Player->EnableFeedback();
}

void BhapticsLibrary::Lib_DisableFeedback()
{
	if (!IsLoaded)
	{
		return;
	}
	Player->DisableFeedback();
}

void BhapticsLibrary::Lib_ToggleFeedback()
{
	if (!IsLoaded)
	{
		return;
	}
	Player->ToggleFeedback();
}

bool BhapticsLibrary::Lib_IsDevicePlaying(EPosition Pos)
{
	if (!IsLoaded)
	{
		return false;
	}
	return Player->IsDeviceConnected(Pos);
}

TArray<FHapticFeedback> BhapticsLibrary::Lib_GetResponseStatus()
{
	TArray<FHapticFeedback> ChangedFeedback;
	if (!IsLoaded)
	{
		return ChangedFeedback;
	}
	
	ChangedFeedback = Player->GetResponseStatus();

	return ChangedFeedback;
}

