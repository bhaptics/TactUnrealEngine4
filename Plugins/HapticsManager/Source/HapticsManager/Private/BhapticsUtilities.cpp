//Copyright bHaptics Inc. 2018

#include "HapticsManager.h"
#include "BhapticsUtilities.h"
#include "IPluginManager.h"

#if _WIN64
#define USEDLL64
#endif

typedef char*(*_GetExePath)();
typedef bool(*_IsPlayerInstalled)();
typedef bool(*_IsPlayerRunning)();
typedef void(*_LaunchPlayer)();
typedef bool(*_IsExternalPlayerRunning)(char* file);

_IsPlayerInstalled m_IsPlayerInstalled;
_IsPlayerRunning m_IsPlayerRunning;
_LaunchPlayer m_LaunchPlayer;
_GetExePath m_GetExePath;
_IsExternalPlayerRunning m_IsExternalPlayerRunning;

bool BhapticsUtilities::IsInitialised = false;

void *v_dllHandle;

BhapticsUtilities::BhapticsUtilities()
{

}

bool BhapticsUtilities::Initialise()
{
	if (!IsInitialised)
	{
		FString FilePath = IPluginManager::Get().FindPlugin("HapticsManager")->GetBaseDir();
		FString FilePathProject = *FPaths::GameContentDir();

#ifdef USEDLL64
		FilePath.Append("/DLLs/bHapticUtility64.dll");
		FilePathProject.Append("HapticsManager/DLLs/bHapticUtility64.dll");
#else
		FilePath.Append("/DLLs/bHapticUtility32.dll");
#endif

		if (FPaths::FileExists(FilePath))
		{
			v_dllHandle = FPlatformProcess::GetDllHandle(*FilePath);

			if (v_dllHandle != NULL)
			{
				IsInitialised = true;
				return true;
			}
			UE_LOG(LogTemp, Log, TEXT("Loading %s failed."), *FilePath);
		}
		else if (FPaths::FileExists(FilePathProject))
		{
			v_dllHandle = FPlatformProcess::GetDllHandle(*FilePathProject);

			if (v_dllHandle != NULL)
			{
				IsInitialised = true;
				return true;
			}
			UE_LOG(LogTemp, Log, TEXT("Loading %s failed."), *FilePathProject);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Could not find dll in %s"), *FilePath);
		}
	}
	return false;

}

void BhapticsUtilities::Free()
{
	if (v_dllHandle != NULL)
	{
		m_IsPlayerInstalled = NULL;
		m_IsPlayerRunning = NULL;
		m_LaunchPlayer = NULL;
		m_GetExePath = NULL;

		FPlatformProcess::FreeDllHandle(v_dllHandle);
		v_dllHandle = NULL;
	}
}

BhapticsUtilities::~BhapticsUtilities()
{

}

FString BhapticsUtilities::GetExecutablePath()
{
	if (v_dllHandle != NULL)
	{
		m_GetExePath = NULL;

		FString ProcName = "getExePath";
		m_GetExePath = (_GetExePath)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_GetExePath != NULL)
		{
			char* returnVal = m_GetExePath();
			FString ReturnString = FString(returnVal);
			return ReturnString;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Executable Path Function Failed"));
	return"";
}

bool BhapticsUtilities::IsPlayerInstalled()
{
	if (v_dllHandle != NULL)
	{
		m_IsPlayerInstalled = NULL;

		FString ProcName = "isPlayerInstalled";
		m_IsPlayerInstalled = (_IsPlayerInstalled)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_IsPlayerInstalled != NULL)
		{
			return m_IsPlayerInstalled();
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Install Check Failed"));
	return false;
}

bool BhapticsUtilities::IsPlayerRunning()
{
	if (v_dllHandle != NULL)
	{
		m_IsPlayerRunning = NULL;
		FString ProcName = "isPlayerRunning";
		m_IsPlayerRunning = (_IsPlayerRunning)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_IsPlayerRunning != NULL)
		{
			return m_IsPlayerRunning();
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Running Check Failed"));
	return false;
}

void BhapticsUtilities::LaunchPlayer()
{

	if (v_dllHandle != NULL)
	{
		m_LaunchPlayer = NULL;

		FString ProcName = "launchPlayer";
		m_LaunchPlayer = (_LaunchPlayer)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_LaunchPlayer != NULL)
		{
			m_LaunchPlayer();
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Function Failed"));
	}
	UE_LOG(LogTemp, Log, TEXT("Launching Function Failed"));

}

bool BhapticsUtilities::IsExternalPlayerRunning(FString file)
{
	if (v_dllHandle != NULL)
	{
		m_IsExternalPlayerRunning = NULL;

		FString ProcName = "isExternalPlayerRunning";
		m_IsExternalPlayerRunning = (_IsExternalPlayerRunning)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_IsExternalPlayerRunning != NULL)
		{
			char* fileChar = TCHAR_TO_ANSI(*file);
			bool ret = m_IsExternalPlayerRunning(fileChar);
			UE_LOG(LogTemp, Log, TEXT("Set EXE location to %s"), *file);
			return ret;
		}
		UE_LOG(LogTemp, Log, TEXT("Function Failed"));
	}
	return false;
}
