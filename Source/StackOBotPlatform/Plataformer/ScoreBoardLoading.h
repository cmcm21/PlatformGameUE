// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StackOBotPlatform/Public/HUD/HUDMenu.h"
#include "ScoreBoardLoading.generated.h"

class UListView;
class UScoreListEntry;
struct FScoreDataStruct;

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UScoreBoardLoading : public UHUDMenu
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="Load Json")
	void LoadScoreBoard(TMap<FString, FScoreDataStruct> data, FString &feedbackMessage);

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Score list",meta=(BindWidget))
	UListView* scoresList;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UScoreListEntry> listEntryTemplate;
};
