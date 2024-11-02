// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/IHUDWidget.h"
#include "HUDInventorySlot.generated.h"

class UInventoryManagerSubsystem;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDInventorySlot : public UIHUDWidget
{
	GENERATED_BODY()

public: 

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlot(int32 amount);

	UFUNCTION(BlueprintImplementableEvent)
	void InitSlot(FOrbDataStruct data);

	UFUNCTION(BlueprintCallable)
	void Increase() 
	{
		orbCounter++;
		UpdateSlot(orbCounter);
	}
	
	UFUNCTION(BlueprintCallable)
	void Decrease() 
	{
		orbCounter--; 
		UpdateSlot(orbCounter);
	}

	UFUNCTION(BlueprintCallable)
	void UseOrb();
	
	int32 GetOrbCounter() { return orbCounter; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Orb Data")
	FOrbDataStruct slotOrbData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Orb Data")
	int32 orbsAmount  = 0;

	TTuple<int32, int32>	gridIndex;
private:
	void GetInventoryManager();

	int32 orbCounter = 1;
	UInventoryManagerSubsystem* inventoryManager;
};
