// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/Boat.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BuoyancyComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/BoatUI.h"
#include "NiagaraComponent.h"

ABoat::ABoat()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boat"));
	RootComponent = BoatMesh;
	BoatMesh->SetSimulatePhysics(true);
	BoatMesh->SetIsReplicated(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));

	L_WaterFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("L Water FX"));
	L_WaterFX->SetupAttachment(GetRootComponent());
	R_WaterFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("R Water FX"));
	R_WaterFX->SetupAttachment(GetRootComponent());
}


void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	BoatSpeed = (GetVelocity().Size2D()) * 0.036;
	if (BoatUI)
	{
		BoatUI->SetSpeed(BoatSpeed);
	}

	if (R_WaterFX && L_WaterFX && BoatSpeed <= 20.f)
	{
		R_WaterFX->Deactivate();
		L_WaterFX->Deactivate();
	}
}

void ABoat::ApplyMovement(float InputX, float InputY)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (R_WaterFX && L_WaterFX && BoatSpeed > 20.f)
	{
		R_WaterFX->Activate(false);
		L_WaterFX->Activate(false);
	}

	FVector ForwardForce = GetActorForwardVector() * ForceMultiplier * InputY;
	BoatSpeed = GetVelocity().Size2D() * 0.036f;

	float CurrentMaxSpeed = InputY > 0 ? MaxSpeed : MaxReverseSpeed;


	if (BoatSpeed <= CurrentMaxSpeed || InputY <= 0)
	{
		if (InputY < 0 && BoatSpeed > MaxReverseSpeed)
		{
			ForwardForce = FVector::ZeroVector;
		}
		BoatMesh->AddForce(ForwardForce);
	}

	if (FMath::Abs(InputX) > KINDA_SMALL_NUMBER)
	{
		if (InputY < 0.f)
		{
			InputX *= -1;
		}
		float CurrentTurn = FMath::Clamp(InputX * TurnRate, -TurnRate, TurnRate) * DeltaTime;
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CurrentTurn * TurnSmoothness;
		SetActorRotation(NewRotation);
	}

	// Apply lateral damping to reduce drifting
	FVector LateralVelocity = BoatMesh->GetRightVector() * FVector::DotProduct(BoatMesh->GetPhysicsLinearVelocity(), BoatMesh->GetRightVector());
	FVector LateralDampingForce = -LateralVelocity * LateralDampingFactor;
	BoatMesh->AddForce(LateralDampingForce);

	X = InputX;
	Y = InputY;
}

void ABoat::Drive(float InputX, float InputY)
{
	ApplyMovement(InputX, InputY);

	if (!HasAuthority())
	{
		ServerDrive(InputX, InputY); 
	}
}

void ABoat::ServerDrive_Implementation(float InputX, float InputY)
{
	ApplyMovement(InputX, InputY);
}

void ABoat::UpdateCheckPoint(const FName& CurrentBoxOverlapTag)
{
	if (NextCheckPoint == TotalCheckPoints)
	{
		CurrentLap++;
		if (BoatUI)
		{
			BoatUI->SetCurrentLap(CurrentLap);
		}
		NextCheckPoint = 1;
		if (Tags.Num() > 0)
		{
			FName FirstTag = Tags[0];
			Tags.Empty();
			Tags.Add(FirstTag);
		}
		return;
	}

	if (Tags.Num() > 0)
	{
		if (CurrentBoxOverlapTag.ToString().Contains(FString::FromInt(NextCheckPoint)))
		{
			Tags.Add(CurrentBoxOverlapTag);
			NextCheckPoint++;
		}
	}
}

void ABoat::UpdateMaxCheckPoints(int32 MaxCheckPoints)
{
	TotalCheckPoints = MaxCheckPoints;
}

void ABoat::UpdateTotalLaps(int32 LevelTotalLaps)
{
	TotalLaps = LevelTotalLaps;
	if (BoatUI)
	{
		BoatUI->SetTotalLaps(TotalLaps);
	}
}


void ABoat::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Boat"));

	if (BoatUIClass) 
	{
	  BoatUI = Cast<UBoatUI>(CreateWidget<UUserWidget>(GetWorld(), BoatUIClass));
		if (BoatUI)
		{
			BoatUI->AddToViewport();
			BoatUI->SetCurrentLap(CurrentLap);
		}
	}

	if (R_WaterFX && L_WaterFX)
	{
		R_WaterFX->Deactivate();
		L_WaterFX->Deactivate();
	}
}
