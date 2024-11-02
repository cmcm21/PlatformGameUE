// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDStaticWidget.h"
#include "StackOBotPlatform/GameModeData.h"
#include "HUDInventoryShortcut.generated.h"

class UHUDInventoryShortCutSlot;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDInventoryShortcut : public UHUDStaticWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	
	TSet<FOrbID> currentShortCuts;
};
