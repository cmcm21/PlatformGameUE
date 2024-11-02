// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDStaticWidget.h"
#include "SaveButton.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API USaveButton : public UHUDStaticWidget
{
	GENERATED_BODY()
public:
	USaveButton();
	virtual bool Initialize() override;

	virtual void Hide() override;
	virtual void Show() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	UButton* saveButton;

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchButtonColor();

};
