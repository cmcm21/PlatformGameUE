// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StackOBotPlatform/Character/PlatformCharacter.h"

struct FInputActionValue;
/**
 * 
 */
class STACKOBOTPLATFORM_API IState
{
public:
	IState();
	virtual ~IState() = 0;

	virtual void Enter(APlatformCharacter *character) = 0;
	virtual void HandleMoveInput(APlatformCharacter *character,const FInputActionValue& inputValue ) = 0;
	virtual void HandleLookInput(APlatformCharacter *character, const FInputActionValue& inputValue) = 0;
	virtual void HandleJumpInput(APlatformCharacter *character, const FInputActionValue &inputValue) = 0;
	virtual void Exit() = 0;
};
