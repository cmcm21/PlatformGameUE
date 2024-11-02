// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataLoadingWidget.h"
#include "MyGameDataSavingWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UMyGameDataSavingWidget : public UGameDataLoadingWidget
{
	GENERATED_BODY()
	
public:
	virtual void GoBack() override;
};
