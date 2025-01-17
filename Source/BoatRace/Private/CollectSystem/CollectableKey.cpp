#include "CollectSystem/CollectableKey.h"
#include "Boat/Boat.h"
#include "Interfaces/BoatInterface.h"
#include "Boat/BoatProperties.h"
#include "CollectSystem/MovableActor.h"

ACollectableKey::ACollectableKey()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    RootComponent = SphereComp;
    SphereComp->SetSphereRadius(100.0f);

    KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
    KeyMesh->SetupAttachment(RootComponent);

    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACollectableKey::HandleOverlap);
}

void ACollectableKey::BeginPlay()
{
    Super::BeginPlay();
}

void ACollectableKey::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    KeyMesh->AddLocalRotation(FRotator(0.f, 100.f * DeltaTime, 0.f)); 
}

void ACollectableKey::HandleOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        IBoatInterface* BoatInterface = Cast<IBoatInterface>(OtherActor);
        if (!BoatInterface) return;
        switch (KeyType)
        {
        case EKeyType::Boost:
            BoatInterface->SetBoostActive(true, FillFuel);
            break;
        case EKeyType::MovableActor:
            if (MovableActor)
            {
                MovableActor->TriggerMovement();
            }
            break;
        default:
            break;
        }
        OnKeyCollected.Broadcast(); 
        Destroy();
    }

}

