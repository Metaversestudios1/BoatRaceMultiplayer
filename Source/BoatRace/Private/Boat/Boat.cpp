// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/Boat.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BuoyancyComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/BoatUI.h"

ABoat::ABoat()
{
	PrimaryActorTick.bCanEverTick = true;

	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boat"));
	RootComponent = BoatMesh;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));

	BoatMesh->SetSimulatePhysics(true);
}


void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	BoatSpeed = (GetVelocity().Size2D()) * 0.036;
	if (BoatUI)
	{
		BoatUI->SetSpeed(BoatSpeed);
	}
}


void ABoat::Drive(float InputX, float InputY)
{
	if (BoatMesh == nullptr) return;

	FVector ForwardForce = ((GetActorForwardVector() * ForceMultiplier) * InputY);
	BoatMesh->AddForce(ForwardForce);

	if (InputY < 0)
	{
		InputX *= -1;
	}
	FRotator TurnRotation = FRotator(0, (InputX * TorqueMultiplier), 0);
	BoatMesh->AddTorqueInDegrees(FVector(0.f, 0.f, TurnRotation.Yaw));

	X = InputX;
	Y = InputY;
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
}