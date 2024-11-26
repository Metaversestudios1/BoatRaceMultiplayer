#include "Boat/BoatProperties.h"

UBoatProperties::UBoatProperties()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UBoatProperties::BeginPlay()
{
	Super::BeginPlay();
	TempForceMultiplier = ForceMultiplier;
}

void UBoatProperties::BoostActivate(bool bBoostActive)
{
	if (bBoostActive)
	{
		ForceMultiplier *= BoostMultiplier;
	}
	else
	{
		ForceMultiplier = TempForceMultiplier;
	}
}

