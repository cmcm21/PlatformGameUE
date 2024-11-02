// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StackOBotPlatform/GameModeData.h"
#include "GameDataSaveGame.generated.h"

struct FLevelDataStruct;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UGameDataSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category=Basic)	
	FGameplayDataStruct gameplayData;

	UPROPERTY(VisibleAnywhere, Category=Basic)
	FString saveSlotName;

	UPROPERTY(VisibleAnywhere, Category=Basic)
	uint32 userIndex;

	UGameDataSaveGame();
};
