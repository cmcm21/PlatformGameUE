// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDStaticWidget.h"
#include "HUDCollectables.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDCollectables : public UHUDStaticWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock* itemsText;
	void UpdateItemsText(int32 totalItems, int32 itemsCollected);
};
