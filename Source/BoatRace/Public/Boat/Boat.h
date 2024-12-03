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
	virtual void PostInitializeComponents() override;
	virtual void UpdateCheckPoint(const FName& CurrentBoxOverlapTag) override;
	virtual void UpdateMaxCheckPoints(int32 MaxCheckPoints) override;
	virtual void UpdateTotalLaps(int32 LevelTotalLaps) override;
	virtual void RotateBoat(float InputY, float InputX) override;
	virtual void SetHandbrakeActive(bool bActive)  override;
	virtual void SetBoostActive(bool bBoostActive) override;

	UFUNCTION(BlueprintCallable)
	virtual void Drive(float InputX, float InputY) override;

	UFUNCTION(Server, Unreliable)
	void ServerDrive(float InputX, float InputY);

	UPROPERTY(BlueprintReadOnly)
	float X = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float Y = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* BoostTrailEffect;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInAir = false;

	UPROPERTY(VisibleAnywhere)
	class UBoatProperties* BoatProperties;


protected:
	virtual void BeginPlay() override;


private:
	void ApplyMovement(float InputX, float InputY);
	void CheckIfInAir();
	void CountDownTransition(float DeltaTime);
	void UpdateBoostFuelUI();


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BoatCharacter;

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


	//VFX
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* L_WaterFX;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* R_WaterFX;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* L_BackWaterFX;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* R_BackWaterFX;

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
private:
	bool bIsHandbrakeActive = false;  
	void ApplyGradualDeceleration(float DeltaTime);  
	void ApplyDrift(float DeltaTime);

	float DecelerationRate = 500.f; //Determines how quickly forward momentum is reduced.
	float CurrentSpeed = 0.f; 

	float LateralDriftForce = 1500.f; //Controls how intense the drift effect is.
	float MinDriftSpeed = 4.f; //Prevents the boat from coming to a complete stop during a drift.
};