#include "Boat/BoostComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UBoostComponent::UBoostComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UBoostComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBoostComponent::ActivateBoost()
{
	if (!bIsBoosting)
	{
		bIsBoosting = true;

		GetWorld()->GetTimerManager().SetTimer(BoostTimerHandle, this, &UBoostComponent::StopBoost, BoostDuration, false);
		UE_LOG(LogTemp, Log, TEXT("Boost Activated!"));
	}
}

void UBoostComponent::DeactivateBoost()
{
	StopBoost(); 
}

void UBoostComponent::StopBoost()
{
	bIsBoosting = false;
	GetWorld()->GetTimerManager().ClearTimer(BoostTimerHandle);

	UE_LOG(LogTemp, Log, TEXT("Boost Deactivated!"));
}