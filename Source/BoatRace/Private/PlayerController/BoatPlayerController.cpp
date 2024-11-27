// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BoatPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interfaces/BoatInterface.h"

void ABoatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem && BoatMappingContext)
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
		if (IA_Handbrake)
		{
			EnhancedInputComponent->BindAction(IA_Handbrake, ETriggerEvent::Triggered, this, &ABoatPlayerController::ActivateHandbrake);
			EnhancedInputComponent->BindAction(IA_Handbrake, ETriggerEvent::Completed, this, &ABoatPlayerController::DeactivateHandbrake);
		}
		if (IA_Boost)
		{
			EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &ABoatPlayerController::ActivateBoost);
			EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &ABoatPlayerController::DeactivateBoost);
		}
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ThisClass::Rotate);
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
	FVector2D MoveAxis = Value.Get<FVector2D>();
	float InputX = MoveAxis.X; 
	float InputY = MoveAxis.Y; 

	if (IsInputKeyDown(EKeys::W) && IsInputKeyDown(EKeys::S))
	{
		InputY = 0.f; 
	}

	if (IsInputKeyDown(EKeys::A) && IsInputKeyDown(EKeys::D))
	{
		InputX = 0.f;
	}
	
	if (BoatInterface)
	{
		BoatInterface->Drive(InputX, InputY);
	}
}


void ABoatPlayerController::ActivateHandbrake()
{
	if (BoatInterface)
	{
		BoatInterface->SetHandbrakeActive(true);
	}
}

void ABoatPlayerController::DeactivateHandbrake()
{
	if (BoatInterface)
	{
		BoatInterface->SetHandbrakeActive(false);
	}
}

void ABoatPlayerController::Rotate(const FInputActionValue& Value)
{
	const FVector2D RotateAxis = Value.Get<FVector2D>();

	if (BoatInterface)
	{
		BoatInterface->RotateBoat(RotateAxis.Y, RotateAxis.X);
	}
}

void ABoatPlayerController::ActivateBoost()
{
	if (BoatInterface)
	{
		BoatInterface->SetBoostActive(true);
	}
}

void ABoatPlayerController::DeactivateBoost()
{
	if (BoatInterface)
	{
		BoatInterface->SetBoostActive(false);
	}
}
