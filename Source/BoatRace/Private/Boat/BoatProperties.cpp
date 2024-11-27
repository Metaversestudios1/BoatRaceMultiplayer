
#include "Boat/BoatProperties.h"
#include "TimerManager.h"


UBoatProperties::UBoatProperties()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UBoatProperties::BeginPlay()
{
	Super::BeginPlay();
	TempForceMultiplier = ForceMultiplier;
	TempMaxSpeed = MaxSpeed;
	
}

void UBoatProperties::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bFillFuel) 
	{
		CurrentBoostFuel += 5.f * DeltaTime;
		CurrentBoostFuel = FMath::Clamp(CurrentBoostFuel, 0.f, MaxBoostFuel);
		if(CurrentBoostFuel == MaxBoostFuel) bFillFuel = false;
	}
	if (CurrentBoostFuel == 0)
	{
		ForceMultiplier = TempForceMultiplier;
		MaxSpeed = TempMaxSpeed;
	}
}

void UBoatProperties::BoostActivate(bool bBoostActive)
{
	if (bBoostActive)
	{
		if (!bIsBoosting) 
		{
			bIsBoosting = true;
			ForceMultiplier *= BoostMultiplier;
			MaxSpeed = BoostMaxSpeed;
		}
		CurrentBoostFuel -= 25.f * GetWorld()->GetDeltaSeconds();
		CurrentBoostFuel = FMath::Clamp(CurrentBoostFuel, 0.f, MaxBoostFuel);
		bFillFuel = false;
	}
	else
	{
		MaxSpeed = TempMaxSpeed;
		bIsBoosting = false;
		ForceMultiplier = TempForceMultiplier;
		GetWorld()->GetTimerManager().SetTimer(BoostTimerHandle, this, &UBoatProperties::EndBoost, BoostCooldown, false);
	}
}

void UBoatProperties::EndBoost()
{
	UE_LOG(LogTemp, Warning, TEXT("Filling Fuel"));
	bFillFuel = true;
}
