// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BoatCharacterAnimInstance.h"
#include "Boat/Boat.h"


void UBoatCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Boat = Cast<ABoat>(TryGetPawnOwner());
}

void UBoatCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Boat == nullptr) return;
	
	if (Boat->GetBoatHandle() && Boat->GetBoatCharacter())
	{
		LeftHandTransform = Boat->GetBoatHandle()->GetSocketTransform(FName("LH_Socket"), ERelativeTransformSpace::RTS_World);
		FVector LOutPosition;
		FRotator LOutRotation;
		Boat->GetBoatCharacter()->TransformToBoneSpace(FName("Spine1_M"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, LOutPosition, LOutRotation);
		LeftHandTransform.SetLocation(LOutPosition);
		LeftHandTransform.SetRotation(FQuat(LOutRotation));

		RightHandTransform = Boat->GetBoatHandle()->GetSocketTransform(FName("RH_Socket"), ERelativeTransformSpace::RTS_World);
		FVector ROutPosition;
		FRotator ROutRotation;
		Boat->GetBoatCharacter()->TransformToBoneSpace(FName("Spine1_M"), RightHandTransform.GetLocation(), FRotator::ZeroRotator, ROutPosition, ROutRotation);
		RightHandTransform.SetLocation(ROutPosition);
		RightHandTransform.SetRotation(FQuat(ROutRotation));
	}
}
