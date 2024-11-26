
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
}

void UBoatProperties::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsBoosting && CurrentBoostFuel > 0.f)
	{
		CurrentBoostFuel -= 20.f * DeltaTime;
		CurrentBoostFuel = FMath::Clamp(CurrentBoostFuel, 0.f, MaxBoostFuel);

		if (CurrentBoostFuel <= 0.f)
		{
			EndBoost();
		}
	}
	else if (!bIsBoosting && !bIsCooldownActive)
	{
		CurrentBoostFuel += 10.f * DeltaTime;
		CurrentBoostFuel = FMath::Clamp(CurrentBoostFuel, 0.f, MaxBoostFuel);
	}
}

void UBoatProperties::BoostActivate(bool bBoostActive)
{
	if (bBoostActive && !bIsCooldownActive && CurrentBoostFuel > 0.f)
	{
		bIsBoosting = true;
		ForceMultiplier *= BoostMultiplier;
		GetWorld()->GetTimerManager().SetTimer(BoostTimerHandle, this, &UBoatProperties::EndBoost, BoostDuration, false);
	}
	else
	{
		EndBoost();
	}
}

void UBoatProperties::EndBoost()
{
	if (!bIsBoosting) return;
	ForceMultiplier = TempForceMultiplier;
	bIsCooldownActive = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UBoatProperties::EndCooldown, BoostCooldown, false);
}

void UBoatProperties::EndCooldown()
{
	bIsCooldownActive = false;
}
