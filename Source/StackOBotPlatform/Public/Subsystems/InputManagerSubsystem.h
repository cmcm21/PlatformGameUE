// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UInputManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	DECLARE_EVENT_OneParam(UInputManagerSubsystem, FShortCutPressedEvent, int32)
	FShortCutPressedEvent &OnShortCutPressed() { return shortCutEvent; }


	void TriggerShortCutEvent(int32 keyId);

private:

	FShortCutPressedEvent shortCutEvent;
};
