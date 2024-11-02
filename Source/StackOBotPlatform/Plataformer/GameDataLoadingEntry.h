// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "GameDataLoadingEntry.generated.h"

struct FSaveSlotMetaData;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UGameDataLoadingEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetData(const FSaveSlotMetaData& slotData);
};
