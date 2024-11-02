// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StackOBotPlatform/Public/HUD/HUDMenu.h"
#include "GameoverWidget.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;

UCLASS()
class STACKOBOTPLATFORM_API UGameoverWidget : public UHUDMenu
{
	GENERATED_BODY()

public:	

	DECLARE_EVENT(UItemsPlatformUserWidget, FResetEvent)
	FResetEvent& OnResetButtonPressed() { return onResetEvent; };

	UFUNCTION(BlueprintCallable)
	void TriggerResetButtonEvent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Binded Components", meta=(BindWidget))
	UTextBlock* mainText;

private:
	FResetEvent onResetEvent;
};
