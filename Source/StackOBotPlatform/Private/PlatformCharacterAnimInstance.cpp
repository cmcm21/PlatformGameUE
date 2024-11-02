// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformCharacterAnimInstance.h"
#include "StackOBotPlatform/Character/PlatformCharacter.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlatformCharacterAnimInstance::NativeInitializeAnimation()
{
	PlatformCharacter = Cast<APlatformCharacter>(TryGetPawnOwner());
	if(PlatformCharacter) PlatformCharacterMovement = PlatformCharacter->GetCharacterMovement();
}

void UPlatformCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (PlatformCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlatformCharacterMovement->Velocity);
		bIsFalling = PlatformCharacterMovement->IsFalling();
		CharacterState = PlatformCharacter->GetCharacterState();
	}
}


