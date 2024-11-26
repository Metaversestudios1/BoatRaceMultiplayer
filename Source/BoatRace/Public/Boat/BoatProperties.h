
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

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float LateralDampingFactor = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float MaxSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float MaxReverseSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float TurnRate = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float TurnSmoothness = 25.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	FVector BoatGravity = FVector(0.f, 0.f, -700.f);

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float BoatTurnSink = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float HandleRotationRate = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float BoatPitchRotation = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float BoatRollRotation = 5.f;

};