// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/BoatInterface.h"
#include "Boat.generated.h"


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

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatMesh;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UBuoyancyComponent* Buoyancy;

	float BoatSpeed = 0;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float ForceMultiplier = 80000.f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float TorqueMultiplier = 400000000.f;

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
};
