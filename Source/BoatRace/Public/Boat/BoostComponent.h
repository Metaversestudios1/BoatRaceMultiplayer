// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoostComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOATRACE_API UBoostComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBoostComponent();

protected:
	virtual void BeginPlay() override;


public: 
	void ActivateBoost();
	void DeactivateBoost();
	float BoostMultiplier = 20.0f;
	bool IsBoostActive() const { return bIsBoosting; }

private:

	bool bIsBoosting = false;
	float BoostDuration = 2.0f;
	FTimerHandle BoostTimerHandle;

	void StopBoost();
		
};
