// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StackOBotPlatform/GameModeData.h"
#include "IHUDWidget.generated.h"

class UHUDManager;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UIHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	HUDWidgetID widgetID;

	virtual void Hide() {};
	virtual void Show() {};
};
