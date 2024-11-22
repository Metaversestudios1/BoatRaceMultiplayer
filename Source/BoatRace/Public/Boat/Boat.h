// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/BoatInterface.h"
#include "BuoyancyTypes.h"
#include "Boat.generated.h"

class UNiagaraComponent;

UCLASS()
class BOATRACE_API ABoat : public APawn, public IBoatInterface
{
	GENERATED_BODY()

public:
	ABoat();
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateCheckPoint(const FName& CurrentBoxOverlapTag) override;
	virtual void UpdateMaxCheckPoints(int32 MaxCheckPoints) override;
	virtual void UpdateTotalLaps(int32 LevelTotalLaps) override;
	virtual void RotateBoat(float InputY, float InputX) override;

	UFUNCTION(BlueprintCallable)
	virtual void Drive(float InputX, float InputY) override;

	UFUNCTION(Server, Unreliable)
	void ServerDrive(float InputX, float InputY);


	UPROPERTY(BlueprintReadOnly)
	float X = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float Y = 0.f;


protected:
	virtual void BeginPlay() override;


private:
	void ApplyMovement(float InputX, float InputY);
	void CheckIfInAir();
	void CountDownTransition(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatMesh;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	float CountDown = 3.f;
	bool bIsCountDownTransitionDone = false;
	FTimerHandle CountTimer;
	void TransitionDone();

	UPROPERTY(VisibleAnywhere)
	class UBuoyancyComponent* Buoyancy;

	FSphericalPontoon* FirstPontoon;
	float FirstPontoonZLoc;

	FSphericalPontoon* ThirdPontoon;
	float ThirdPontoonZLoc;

	FSphericalPontoon* SecondPontoon;
	float SecondPontoonZLoc;

	FSphericalPontoon* FourthPontoon;
	float FourthPontoonZLoc;

	FTimerHandle BuoyancyTimer;
	UPROPERTY(VisibleAnywhere)
	bool bSetBuoyancyData = false;
	void SetBuoyancyData();

	float BoatSpeed = 0;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* L_WaterFX;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* R_WaterFX;	
	
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* L_BackWaterFX;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* R_BackWaterFX;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float ForceMultiplier = 80000.f;

	UPROPERTY(EditAnywhere)
	float LateralDampingFactor = 100.f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float MaxSpeed = 80.f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float MaxReverseSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float TurnRate = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float TurnSmoothness = 25.f;

	bool bIsInAir = false;

	UPROPERTY(VisibleInstanceOnly, Category = "Lap")
	int32 NextCheckPoint = 1;

	UPROPERTY(VisibleInstanceOnly, Category = "Lap")
	int32 TotalCheckPoints = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Lap")
	int32 CurrentLap = 1;

	UPROPERTY(VisibleInstanceOnly, Category = "Laps")
	int32 TotalLaps = 5;

	UPROPERTY()
	class UBoatUI* BoatUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BoatUIClass;


	/****HandBrake****/
public:
	void SetHandbrakeActive(bool bActive);

private:
	bool bIsHandbrakeActive = false;  
	void ApplyGradualDeceleration(float DeltaTime);  
	void ApplyDrift(float DeltaTime);

	float DecelerationRate = 500.f; 
	float CurrentSpeed = 0.f; 

	float LateralDriftForce = 1500.f;
	float MinDriftSpeed = 4.f;


};
