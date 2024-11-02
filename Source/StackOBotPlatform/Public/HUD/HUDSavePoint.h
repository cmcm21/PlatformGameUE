// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/IHUDWidget.h"
#include "HUDSavePoint.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDSavePoint : public UIHUDWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Notify();
};
