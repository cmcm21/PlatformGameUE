// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDToggleableWidget.h"
#include "HUD/HUDInventorySlot.h"
#include "StackOBotPlatform/GameModeData.h"
#include "Containers/Map.h"
#include "HUDInventory.generated.h"

const int32 MAX_COLUMN = 2;
const int32 MAX_ROW = 6;

class UVerticalBox;
class UInventoryManagerSubsystem;

 

UCLASS()
class STACKOBOTPLATFORM_API UHUDInventory : public UHUDToggleableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bind Widgets", meta=(BindWidget))
	UVerticalBox * inventoryGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot")
	TSubclassOf<class UHUDInventorySlot> BPWInventorySlot;

private:
	TMap<FOrbID, UHUDInventorySlot*> inventorySlots;
	TSet<TTuple<int32,int32>> gridIndexes;
	UInventoryManagerSubsystem *inventorySys;

	//callbacks
	void OnInventoryGrown(FOrbDataStruct data);
	void OnInventoryShrank(FOrbID orbID);
	void OnInventoryLoaded(FInventoryStruct inventoryData);

	void GetInventoryManagerSys();
	void AddOrb(FOrbDataStruct orbData);
	UHUDInventorySlot *CreateInventorySlot();
	void AddToGrid(UHUDInventorySlot* slot);
	void RemoveFromGrid(UHUDInventorySlot* slot);
	void RemoveOrb(FOrbID orbID);
};
