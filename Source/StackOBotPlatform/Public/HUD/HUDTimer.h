// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDStaticWidget.h"
#include "HUDTimer.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDTimer : public UHUDStaticWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *timerText;
	
	UFUNCTION(BlueprintCallable)
	void UpdateTimer(float time);
};
