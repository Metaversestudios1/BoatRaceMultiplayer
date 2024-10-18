// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BoatRaceGameMode.h"
#include "LapSystem/LapCounter.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/BoatInterface.h"

void ABoatRaceGameMode::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<ALapCounter> LapCounter = ALapCounter::StaticClass();
	TArray<AActor*> FoundAllCheckPoints;
	UGameplayStatics::GetAllActorsOfClass(this, LapCounter, FoundAllCheckPoints);
	if (FoundAllCheckPoints.Num() > 0)
	{
		for (const AActor* CheckPoint : FoundAllCheckPoints)
		{
			UE_LOG(LogTemp, Warning, TEXT("CheckPoint Name: %s"), *CheckPoint->GetName());
		}

		TArray<AActor*> BoatActors;
		UGameplayStatics::GetAllActorsWithInterface(this, UBoatInterface::StaticClass(), BoatActors);
		
		if (BoatActors.Num() > 0)
		{
			for (AActor* BoatActor : BoatActors)
			{
				if (BoatActor->GetClass()->ImplementsInterface(UBoatInterface::StaticClass()))
				{
					IBoatInterface* BoatInterface = Cast<IBoatInterface>(BoatActor);
					if (BoatInterface)
					{
						BoatInterface->UpdateMaxCheckPoints(FoundAllCheckPoints.Num());
						BoatInterface->UpdateTotalLaps(TotalLaps);
					}
				}
			}
		}
	}
}