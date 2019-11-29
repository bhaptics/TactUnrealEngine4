//Copyright bHaptics Inc. 2017 - 2019
#include "HapticDevice.h"
#include "AndroidHapticLibrary.h"

void UHapticDevice::SetDeviceData(FDevice LinkedDevice)
{
	DeviceData = LinkedDevice;
	IsInitialised = true;
	DidUpdateDevice = true;
}

void UHapticDevice::Pair()
{
	if (IsInitialised)
	{
		UAndroidHapticLibrary::AndroidThunkCpp_Pair(DeviceData.Address);
	}
}

void UHapticDevice::Unpair()
{
	if (IsInitialised)
	{
		UAndroidHapticLibrary::AndroidThunkCpp_Unpair(DeviceData.Address);
	}
}

void UHapticDevice::Ping()
{
	if (IsInitialised)
	{
		UAndroidHapticLibrary::AndroidThunkCpp_Ping(DeviceData.Address);
	}
}

void UHapticDevice::ChangePosition(FString Position)
{
	if (IsInitialised)
	{
		UAndroidHapticLibrary::AndroidThunkCpp_ChangePosition(DeviceData.Address, Position);
	}
}

void UHapticDevice::TogglePosition()
{
	if (IsInitialised)
	{
		UAndroidHapticLibrary::AndroidThunkCpp_TogglePosition(DeviceData.Address);
	}
}
