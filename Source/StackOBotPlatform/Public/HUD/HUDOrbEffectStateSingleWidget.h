// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/IHUDWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "HUDOrbEffectStateSingleWidget.generated.h"

class URichTextBlock;
struct FOrbDataStruct;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDOrbEffectStateSingleWidget : public UIHUDWidget, public IUserListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitWidget(FOrbDataStruct data);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTimer(float time);
};
