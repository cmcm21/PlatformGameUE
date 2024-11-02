// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/IHUDWidget.h"
#include "HUDOrbsInfo.generated.h"

class URichTextBlock;
class UWidgetAnimation;
struct FOrbDataStruct;

UCLASS()
class STACKOBOTPLATFORM_API UHUDOrbsInfo : public UIHUDWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Notify(FOrbDataStruct orbData);

	void Notify(FText notifyTxt);

	UPROPERTY(EditDefaultsOnly,meta=(BindWidget))
	URichTextBlock* infoText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool animating = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowAnimation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FOrbDataStruct> animationsQueue;
};
