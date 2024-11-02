// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "StackOBotPlatform/GameModeData.h"
#include "HUDMenu.generated.h"

class UHUDManager;
class APlatformHUD;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	HUDMenuID menuID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlatformHUD *platformHUD;

	UFUNCTION(BlueprintCallable)
	virtual void GoBack();
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
};
