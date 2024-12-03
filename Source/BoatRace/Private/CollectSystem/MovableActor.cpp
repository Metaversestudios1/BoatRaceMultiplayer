#include "CollectSystem/MovableActor.h"

AMovableActor::AMovableActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovableMesh"));
    RootComponent = MovableMesh;

    bIsMoving = false;
    Movetime = 2.0f;
}

void AMovableActor::BeginPlay()
{
    Super::BeginPlay();

    StartPoint = GetActorLocation();
    EndPoint = StartPoint + FVector(0.f, 0.f, 500.f); 
}

void AMovableActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

   if (bIsMoving)
   {
        FVector CurrentLocation = GetActorLocation();
        float Speed = FVector::Distance(StartPoint, EndPoint) / Movetime;
        FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, EndPoint, DeltaTime, Speed);
        SetActorLocation(NewLocation);

        if (FVector::Dist(NewLocation, EndPoint) <= KINDA_SMALL_NUMBER)
        {
            bIsMoving = false;
            GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AMovableActor::ResetPosition, ResetTime, false);
        }
   }
}

void AMovableActor::TriggerMovement()
{
    bIsMoving = true;
}

void AMovableActor::ResetPosition()
{
    SetActorLocation(StartPoint);
}
