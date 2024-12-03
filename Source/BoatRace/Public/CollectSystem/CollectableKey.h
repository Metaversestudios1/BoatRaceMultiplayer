#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CollectableKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyCollected);

UENUM(BlueprintType)
enum class EKeyType : uint8
{
    Boost UMETA(DisplayName = "Boost"),
    MovableActor UMETA(DisplayName = "MovableActor")
};

UCLASS()
class BOATRACE_API ACollectableKey : public AActor
{
    GENERATED_BODY()

public:
    ACollectableKey();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnKeyCollected OnKeyCollected;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereComp;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* KeyMesh;

    UPROPERTY(EditAnywhere, Category = "Key Settings")
    EKeyType KeyType;

    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere)
    class AMovableActor* MovableActor;
};
