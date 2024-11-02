// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreListEntry.generated.h"

class UTextBlock;
struct FScoreDataStruct;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UScoreListEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetData(int32 listNumber,const FScoreDataStruct &gameplayStruct);
};
