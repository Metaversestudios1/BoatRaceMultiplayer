// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BoatUI.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


void UBoatUI::SetSpeed(float BoatSpeed)
{
	if (Speed)
	{
		BoatSpeed = FMath::RoundToInt(BoatSpeed);
		Speed->SetText(FText::AsNumber(BoatSpeed));
	}
}

void UBoatUI::SetCurrentLap(int32 BoatCurrentLap)
{
	if (CurrentLap)
	{
		CurrentLap->SetText(FText::AsNumber(BoatCurrentLap));
	}
}

void UBoatUI::SetTotalLaps(int32 GameTotalLap)
{
	if (TotalLaps)
	{
		TotalLaps->SetText(FText::AsNumber(GameTotalLap));
	}
}

void UBoatUI::UpdateBoostFuel(float FuelLevel)
{
	if (BoostFuelBar)
	{
		float FuelPercent = FuelLevel / 100.f;
		BoostFuelBar->SetPercent(FuelPercent);
	}
}
