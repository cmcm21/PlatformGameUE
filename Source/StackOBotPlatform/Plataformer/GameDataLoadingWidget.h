// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StackOBotPlatform/Public/HUD/HUDMenu.h"
#include "GameDataLoadingWidget.generated.h"

/**
 * 
 */
struct FSaveSlotMetaData;
class UListView;
class UGameDataLoadingEntry;


UCLASS()
class STACKOBOTPLATFORM_API UGameDataLoadingWidget : public UHUDMenu
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Load Game Data")
	void LoadGameData(TMap<FString, FSaveSlotMetaData> data, FText& feedbackMessage); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UListView *loadList;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameDataLoadingEntry> listEntryTemplate;
};
