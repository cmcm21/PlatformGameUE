// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StackOBotPlatform/GameModeData.h"
#include "GI_Platformer.generated.h"


UCLASS()
class STACKOBOTPLATFORM_API UGI_Platformer : public UGameInstance
{
	GENERATED_BODY()
	
public:
	GameoverState gameoverState;

	UFUNCTION(BlueprintCallable)
	void SetGameoverState(const GameoverState& newState);

	UFUNCTION(BlueprintCallable)
	GameoverState& GetGameoverState();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetLastlevelTime(float Time) { LastLevelTime = Time; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetLastLevelTime() { return LastLevelTime; }

private:
	float LastLevelTime = -1;
};
