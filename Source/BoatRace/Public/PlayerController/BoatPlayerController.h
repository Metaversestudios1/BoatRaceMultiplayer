// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BoatPlayerController.generated.h"

class UInputAction;

UCLASS()
class BOATRACE_API ABoatPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:

	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;


private:
	TScriptInterface<class IBoatInterface> BoatInterface;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* BoatMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* DriveAction;
	void Drive(const FInputActionValue& Value);

	//HandBrake
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Handbrake;
	void ActivateHandbrake();
	void DeactivateHandbrake();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RotateAction;
	void Rotate(const FInputActionValue& Value);
};
