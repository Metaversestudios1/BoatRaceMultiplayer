// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/Boat.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BuoyancyComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/BoatUI.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"


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
	
	L_BackWaterFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("L Back Water FX"));
	L_BackWaterFX->SetupAttachment(GetRootComponent());
	R_BackWaterFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("R Back Water FX"));
	R_BackWaterFX->SetupAttachment(GetRootComponent());
}


void ABoat::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Boat"));

	if (BoatUIClass /* && IsLocallyControlled()*/)
	{
		BoatUI = Cast<UBoatUI>(CreateWidget<UUserWidget>(GetWorld(), BoatUIClass));
		if (BoatUI)
		{
			BoatUI->AddToViewport();
			BoatUI->SetCurrentLap(CurrentLap);
		}
	}

	if (R_WaterFX && L_WaterFX && R_BackWaterFX && L_BackWaterFX)
	{
		R_WaterFX->Deactivate();
		L_WaterFX->Deactivate();
		L_BackWaterFX->Deactivate();
		R_BackWaterFX->Deactivate();
	}

	if (Buoyancy && Buoyancy->BuoyancyData.Pontoons.Num() > 0)
	{
		FirstPontoon = &Buoyancy->BuoyancyData.Pontoons[0];
		FirstPontoonZLoc = FirstPontoon->RelativeLocation.Z;

		ThirdPontoon = &Buoyancy->BuoyancyData.Pontoons[2];
		ThirdPontoonZLoc = ThirdPontoon->RelativeLocation.Z;

		SecondPontoon = &Buoyancy->BuoyancyData.Pontoons[1];
		SecondPontoonZLoc = SecondPontoon->RelativeLocation.Z;

		FourthPontoon = &Buoyancy->BuoyancyData.Pontoons[3];
		FourthPontoonZLoc = FourthPontoon->RelativeLocation.Z;
	}
}


void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	BoatSpeed = (GetVelocity().Size2D()) * 0.036;
	if (BoatUI)
	{
		BoatUI->SetSpeed(BoatSpeed);
	}

	CheckIfInAir();

	if (!bIsCountDownTransitionDone)
	{
		CountDownTransition(DeltaTime);
	}

	if (bIsHandbrakeActive)
	{
		ApplyGradualDeceleration(DeltaTime);
	}
}

void ABoat::CheckIfInAir()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, 100.0f);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("WaterTrace")), false, this);
	TraceParams.bReturnPhysicalMaterial = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (bHit)
	{
		FString ActorName =  HitResult.GetActor()->GetName();
		if (ActorName.Contains(TEXT("Water")))
		{
			bIsInAir = false;
			if (R_WaterFX && L_WaterFX && R_BackWaterFX && L_BackWaterFX && BoatSpeed > 20.f)
			{
				R_BackWaterFX->Activate(false);
				L_BackWaterFX->Activate(false);
			}
			else if (R_WaterFX && L_WaterFX && R_BackWaterFX && L_BackWaterFX && BoatSpeed < 20.f)
			{
				R_WaterFX->Deactivate();
				L_WaterFX->Deactivate();
				R_BackWaterFX->Deactivate();
				L_BackWaterFX->Deactivate();
			}
			if (!bSetBuoyancyData)
			{
				GetWorldTimerManager().SetTimer(BuoyancyTimer, this, &ThisClass::SetBuoyancyData, 2.f, false);
				bSetBuoyancyData = true;
			}
			return;
		}
	}

	bIsInAir = true;
	//SpringArm->bUsePawnControlRotation = true;
	R_WaterFX->Deactivate();
	L_WaterFX->Deactivate();
	R_BackWaterFX->Deactivate();
	L_BackWaterFX->Deactivate();
	Buoyancy->BuoyancyData.BuoyancyCoefficient = 0.6;
	bSetBuoyancyData = false;
	GetWorldTimerManager().ClearTimer(BuoyancyTimer);
}

void ABoat::CountDownTransition(float DeltaTime)
{
	CountDown -= DeltaTime;

	FVector TargetOffset;
	FRotator TargetRotation;
	int32 Count = FMath::CeilToInt32(CountDown);

	switch (Count)
	{
	case 3:
		TargetOffset = FVector(1200, 400, -50);
		TargetRotation = FRotator(0, 215, 0);
		break;
	case 2:
		TargetOffset = FVector(1200, 0, -150);
		TargetRotation = FRotator(0, 180, 0);
		break;
	case 1:
		TargetOffset = FVector(750, -800, -100);
		TargetRotation = FRotator(0, 100, 0);
		break;
	default:
		TargetOffset = FVector(0, 0, 0);
		TargetRotation = FRotator(0, 0, 0);
		GetWorldTimerManager().SetTimer(CountTimer, this, &ThisClass::TransitionDone, 1.f, false);
	}

	float InterpSpeed = 2.5f;
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, TargetOffset, DeltaTime, InterpSpeed);
	Camera->SetRelativeRotation(FMath::RInterpTo(Camera->GetRelativeRotation(), TargetRotation, DeltaTime, InterpSpeed));
}

void ABoat::TransitionDone()
{
	bIsCountDownTransitionDone = true;
}

void ABoat::SetBuoyancyData()
{
	//SpringArm->bUsePawnControlRotation = true;
	Buoyancy->BuoyancyData.BuoyancyCoefficient = 0.3;
	bSetBuoyancyData = true;
}

void ABoat::ApplyMovement(float InputX, float InputY)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

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
		FVector BoatVelocity = BoatMesh->GetPhysicsLinearVelocity();
		bool bIsMovingBackward = FVector::DotProduct(BoatVelocity, GetActorForwardVector()) < 0;

		if (InputY < 0.f && bIsMovingBackward)
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
	if (bIsInAir) return;

	ApplyMovement(InputX, InputY);

	float TargetFirstZLoc = FirstPontoonZLoc;
	float TargetThirdZLoc = ThirdPontoonZLoc;
	float TargetSecondZLoc = SecondPontoonZLoc;
	float TargetFourthZLoc = FourthPontoonZLoc;
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (InputX < 0 && BoatSpeed > 20.f)
	{
		L_WaterFX->Activate(false);
		TargetFirstZLoc = FirstPontoonZLoc + 40.f;
		TargetThirdZLoc = ThirdPontoonZLoc + 40.f;
	}
	else if (InputX > 0 && BoatSpeed > 20.f)
	{
		R_WaterFX->Activate(false);
		TargetSecondZLoc = SecondPontoonZLoc + 40.f;
		TargetFourthZLoc = FourthPontoonZLoc + 40.f;
	}
	else
	{
		R_WaterFX->Deactivate();
		L_WaterFX->Deactivate();
	}

	FirstPontoon->RelativeLocation.Z = FMath::FInterpTo(FirstPontoon->RelativeLocation.Z, TargetFirstZLoc, DeltaTime, 2.0f);
	ThirdPontoon->RelativeLocation.Z = FMath::FInterpTo(ThirdPontoon->RelativeLocation.Z, TargetThirdZLoc, DeltaTime, 2.0f);

	SecondPontoon->RelativeLocation.Z = FMath::FInterpTo(SecondPontoon->RelativeLocation.Z, TargetSecondZLoc, DeltaTime, 2.0f);
	FourthPontoon->RelativeLocation.Z = FMath::FInterpTo(FourthPontoon->RelativeLocation.Z, TargetFourthZLoc, DeltaTime, 2.0f);


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

void ABoat::RotateBoat(float InputY, float InputX)
{
	if (!bIsInAir && BoatMesh) return;

	FVector Torque = BoatMesh->GetRightVector() * InputY;
	BoatMesh->AddTorqueInRadians(Torque * 5000000);
	BoatMesh->AddTorqueInRadians(FVector(-InputX * 3000000, 0, 0));
}

/**HandBrake**/
void ABoat::SetHandbrakeActive(bool bActive)
{
	bIsHandbrakeActive = bActive;

	if (bIsHandbrakeActive)
	{
		CurrentSpeed = BoatMesh->GetComponentVelocity().Size();
	}
}

void ABoat::ApplyGradualDeceleration(float DeltaTime)
{
	if (CurrentSpeed > 0.f)
	{
		CurrentSpeed -= DecelerationRate * DeltaTime; 
		CurrentSpeed = FMath::Max(CurrentSpeed, MinDriftSpeed);

		FVector CurrentVelocity = BoatMesh->GetComponentVelocity().GetSafeNormal() * CurrentSpeed;
		BoatMesh->SetPhysicsLinearVelocity(CurrentVelocity);

		ApplyDrift(DeltaTime);
	}
}

void ABoat::ApplyDrift(float DeltaTime)
{
	if (!bIsHandbrakeActive || CurrentSpeed <= MinDriftSpeed)
	{
		return;  
	}

	FVector RightVector = BoatMesh->GetRightVector();

	FVector DriftForce = RightVector * LateralDriftForce * DeltaTime;

	BoatMesh->AddForce(DriftForce, NAME_None, true);
}
