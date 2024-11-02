// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDStaticWidget.h"
#include "HUDInventoryShortCutSlot.generated.h"

struct FOrbDataStruct;
class UInputManagerSubsystem;
class UInventoryManagerSubsystem;

UCLASS()
class STACKOBOTPLATFORM_API UHUDInventoryShortCutSlot : public UHUDStaticWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	DECLARE_EVENT_TwoParams(UHUDInventoryShortCutSlot, FOrbWidgetOnDrop, FOrbID,UHUDInventoryShortCutSlot*)
	FOrbWidgetOnDrop &OnOrbDrop() { return onOrbWidgetDropEvent; } 

	DECLARE_EVENT_TwoParams(UHUDInventoryShortCutSlot, FOrbToInventoryEvent, FOrbID,
		UHUDInventoryShortCutSlot*)
	FOrbToInventoryEvent& OnOrbToInventory() { return toInventoryEvent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Short Cut Key")
	int32 keyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Short Cut OrbData")
	FOrbDataStruct slotOrbData;

	UFUNCTION(BlueprintImplementableEvent)
	void OnShortCutPressed(int32 key);

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemMovedToInventory(int32 slotKey);

	UFUNCTION(BlueprintImplementableEvent)
	void ClearSlot();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInventoryUpdated(int32 newAmount);

private:

	UInputManagerSubsystem* inputManagerSubsys;
	UInventoryManagerSubsystem* inventoryManagerSubsys;
	FOrbWidgetOnDrop onOrbWidgetDropEvent;
	FOrbToInventoryEvent toInventoryEvent;

	void GetInputManagerSubsys();
	void GetInventoryManagerSubsys();
	void OnInventoryUpdatedCallback(FOrbID orbID);
};
