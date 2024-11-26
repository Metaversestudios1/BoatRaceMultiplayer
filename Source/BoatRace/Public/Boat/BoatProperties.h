
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Boat.h"
#include "BoatProperties.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOATRACE_API UBoatProperties : public UActorComponent
{
	GENERATED_BODY()


public:	
	UBoatProperties();
	friend class ABoat;

protected:
	virtual void BeginPlay() override;

private:

	void BoostActivate(bool bBoostActive);

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float ForceMultiplier = 350000.f;
	float TempForceMultiplier = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float BoostMultiplier = 1.2;
};
