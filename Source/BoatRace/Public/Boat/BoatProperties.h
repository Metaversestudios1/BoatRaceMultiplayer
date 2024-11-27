
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



private:

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float ForceMultiplier = 350000.f;
	float TempForceMultiplier = 0.f;


	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float LateralDampingFactor = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boat Movement")
	float MaxSpeed = 150.f;
	float TempMaxSpeed = 0.f;

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

	// Boost Mechanics
	void BoostActivate(bool bBoostActive);
	void EndBoost();

	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	float BoostMultiplier = 1.3;

	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	float BoostCooldown = 3.f;  

	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	float MaxBoostFuel = 100.f;  

	UPROPERTY(VisibleAnywhere, Category = "Boost")
	float CurrentBoostFuel = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Boost")
	float BoostMaxSpeed = 200;

	bool bIsBoosting = false;       
	bool bFillFuel = false; 

	

	FTimerHandle BoostTimerHandle;
};