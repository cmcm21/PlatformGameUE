// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "TimeFormatingSystem.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UTimeFormatingSystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:

	FString GetJustSecondsString(float seconds);
	FString GetJustMilisecondsString(float seconds);
	FString GetJustMinutesString(float seconds);
	FString GetTimeString(float seconds);

private:

	FString stringMinutes;
	FString stringSeconds;
	FString stringMiliseconds;

	void GetSplitTime(float seconds);
};
