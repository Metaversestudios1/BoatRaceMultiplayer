// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BoatPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interfaces/BoatInterface.h"


void ABoatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(BoatMappingContext, 0);
	}

	BoatInterface = TScriptInterface<IBoatInterface>(GetPawn());
}


void ABoatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABoatPlayerController::Look);
		EnhancedInputComponent->BindAction(DriveAction, ETriggerEvent::Triggered, this, &ThisClass::Drive);
	}
}

void ABoatPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	AddYawInput(LookAxis.X);
	AddPitchInput(LookAxis.Y);
}


void ABoatPlayerController::Drive(const FInputActionValue& Value)
{
	const FVector2D MoveAxis = Value.Get<FVector2D>();

	if (BoatInterface)
	{
		BoatInterface->Drive(MoveAxis.X, MoveAxis.Y);
	}
}
