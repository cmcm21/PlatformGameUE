// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "StackOBotPlatform/Character/CharacterTypes.h"
#include "PlatformCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UPlatformCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = PlatformCharacter)
	class APlatformCharacter* PlatformCharacter;

	UPROPERTY(BlueprintReadOnly, Category = PlatformCharacter)
	class UCharacterMovementComponent* PlatformCharacterMovement;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = State)
	ECharacterState CharacterState;
};
