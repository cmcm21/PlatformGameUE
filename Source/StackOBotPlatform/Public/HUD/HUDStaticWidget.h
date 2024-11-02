// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/IHUDWidget.h"
#include "HUDStaticWidget.generated.h"


/**
 * 
 */


UCLASS()
class STACKOBOTPLATFORM_API UHUDStaticWidget : public UIHUDWidget
{
	GENERATED_BODY()

public:
	virtual void Show() override;
	virtual void Hide() override;
};
