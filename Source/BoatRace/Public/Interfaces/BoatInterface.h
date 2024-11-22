// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BoatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBoatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BOATRACE_API IBoatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Drive(float InputX, float InputY) = 0;
	virtual void UpdateCheckPoint(const FName& CurrentBoxOverlapTag) = 0;
	virtual void UpdateMaxCheckPoints(int32 MaxCheckPoints) = 0;
	virtual void UpdateTotalLaps(int32 LevelTotalLaps) = 0;
	virtual void RotateBoat(float InputY, float InputX) = 0;
	virtual void SetHandbrakeActive(bool bActive) = 0;
};
