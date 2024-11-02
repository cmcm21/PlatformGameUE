// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveScore.generated.h"

class UTextBlock;
class UEditableTextBox;
struct FScoreDataStruct;
struct FLevelDataStruct;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API USaveScore : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Score Text")
	UTextBlock *scoreDisplay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Score Text")
	UEditableTextBox *playerName;

	void SetScoreText(float time, int32 falls);

	UFUNCTION(BlueprintCallable, Category="Score")
	FScoreDataStruct GetGameplayScore(bool &bOutSuccess);

	UFUNCTION(BlueprintCallable, Category="Score")
	FLevelDataStruct AddGameNameToGameData(FLevelDataStruct data);

protected:
	int32 gameplayTime = 0;
	int32 gameplayFalls = 0;
};
