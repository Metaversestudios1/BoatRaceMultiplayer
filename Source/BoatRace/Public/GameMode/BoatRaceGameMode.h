// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BoatRaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BOATRACE_API ABoatRaceGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:


protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Lap")
	int32 TotalLaps = 2;
};
