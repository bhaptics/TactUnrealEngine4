// Fill out your copyright notice in the Description page of Project Settings.


#include "BhapticsUtils.h"

BhapticsUtils::BhapticsUtils()
{
}

BhapticsUtils::~BhapticsUtils()
{
}

FString BhapticsUtils::PositionEnumToString(EPosition Position)
{
	FString PositionString = "All";

	switch (Position)
	{
	case EPosition::Left:
		PositionString = "Left";
		break;
	case EPosition::Right:
		PositionString = "Right";
		break;
	case EPosition::Head:
		PositionString = "Head";
		break;
	case EPosition::HandL:
		PositionString = "HandL";
		break;
	case EPosition::HandR:
		PositionString = "HandR";
		break;
	case EPosition::FootL:
		PositionString = "FootL";
		break;
	case EPosition::FootR:
		PositionString = "FootR";
		break;
	case EPosition::ForearmL:
		PositionString = "ForearmL";
		break;
	case EPosition::ForearmR:
		PositionString = "ForearmR";
		break;
	case EPosition::VestFront:
		PositionString = "VestFront";
		break;
	case EPosition::VestBack:
		PositionString = "VestBack";
		break;
	default:
		break;
	}

	return PositionString;
}

FString BhapticsUtils::PositionEnumToDeviceString(EPosition Position)
{
	FString PositionString = "All";

	switch (Position)
	{
	case EPosition::Left:
		PositionString = "Left";
		break;
	case EPosition::Right:
		PositionString = "Right";
		break;
	case EPosition::Head:
		PositionString = "Head";
		break;
	case EPosition::HandL:
		PositionString = "HandL";
		break;
	case EPosition::HandR:
		PositionString = "HandR";
		break;
	case EPosition::FootL:
		PositionString = "FootL";
		break;
	case EPosition::FootR:
		PositionString = "FootR";
		break;
	case EPosition::ForearmL:
		PositionString = "ForearmL";
		break;
	case EPosition::ForearmR:
		PositionString = "ForearmR";
		break;
	case EPosition::VestFront:
		PositionString = "Vest";
		break;
	case EPosition::VestBack:
		PositionString = "Vest";
		break;
	default:
		break;
	}

	return PositionString;
}
