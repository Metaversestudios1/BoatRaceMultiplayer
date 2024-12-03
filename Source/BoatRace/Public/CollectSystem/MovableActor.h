#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "MovableActor.generated.h"

UCLASS()
class BOATRACE_API AMovableActor : public AActor
{
    GENERATED_BODY()

public:
    AMovableActor();
    virtual void Tick(float DeltaTime) override;

    void TriggerMovement();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MovableMesh;

    UPROPERTY(EditAnywhere, Category = "Movement")
    FVector StartPoint;

    UPROPERTY(EditAnywhere, Category = "Movement")  
    FVector EndPoint;

    bool bIsMoving;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float Movetime;

    float ResetTime = 3.0f;
    FTimerHandle ResetTimerHandle;

    void ResetPosition();

};
