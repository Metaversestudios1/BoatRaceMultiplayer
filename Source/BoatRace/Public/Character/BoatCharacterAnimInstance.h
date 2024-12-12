// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BoatCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BOATRACE_API UBoatCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;


private:
	UPROPERTY(BlueprintReadOnly, Category = "Boat", meta = (AllowPrivateAccess = "true"))
	class ABoat* Boat;

	UPROPERTY(BlueprintReadOnly, Category = "Boat | Character", meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Boat | Character", meta = (AllowPrivateAccess = "true"))
	FTransform RightHandTransform;
};
