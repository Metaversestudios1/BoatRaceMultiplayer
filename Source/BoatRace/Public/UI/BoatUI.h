// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoatUI.generated.h"

class UTextBlock;

UCLASS()
class BOATRACE_API UBoatUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetSpeed(float BoatSpeed);
	void SetCurrentLap(int32 BoatCurrentLap);
	void SetTotalLaps(int32 GameTotalLap);

	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Speed;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentLap;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalLaps;

};
