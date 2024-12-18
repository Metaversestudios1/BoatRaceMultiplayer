// Fill out your copyright notice in the Description page of Project Settings.


#include "LapSystem/LapCounter.h"

#include "Boat/Boat.h"
#include "Components/BoxComponent.h"
#include "Interfaces/BoatInterface.h"


ALapCounter::ALapCounter()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
}


void ALapCounter::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ALapCounter::OnBoxOverlap);
}

void ALapCounter::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Num() > 0 && Tags.Num() > 0)
	{
		for (const FName& Tag : OtherActor->Tags)
		{
			if (Tag == Tags[0])
			{
				return;
			}
		}
	}

	if (OtherActor->ActorHasTag(FName("Boat")) && Tags.Num() > 0)
	{
		if (IBoatInterface* Boat = Cast<IBoatInterface>(OtherActor))
		{
			Boat->UpdateCheckPoint(Tags[0]);
		}
	}
	if (OtherActor->ActorHasTag(FName("Boat")))
	{
		if (IBoatInterface* Boat = Cast<IBoatInterface>(OtherActor))
		{
			Boat->UpdateCheckPoint(Tags[0]);

			// Update respawn location
			if (ABoat* BoatActor = Cast<ABoat>(OtherActor))
			{
				FVector LapCounterLocation = GetLapCounterLocation();
				BoatActor->SetRespawnLocation(LapCounterLocation);

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1, // Key (unique ID); -1 means always add new message
						5.0f, // Duration in seconds
						FColor::Yellow, // Text color
						FString::Printf(TEXT("LapCounter Location: %s"), *LapCounterLocation.ToString())
					);
				}
			}
		}
	}
}


void ALapCounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ALapCounter::GetLapCounterLocation() const
{
	return GetActorLocation();
}