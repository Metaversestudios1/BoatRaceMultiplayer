// Fill out your copyright notice in the Description page of Project Settings.


#include "LapSystem/LapCounter.h"
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
}


void ALapCounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}