// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StackOBotPlatform/GameModeData.h"
#include "InventoryManagerSubsystem.generated.h"

class UOrbsEffectManagerSubsystem;
class UDataManagerSubsystem;
struct FInventoryStruct;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UInventoryManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UInventoryManagerSubsystem, FInventoryGrownEvent, FOrbDataStruct)
	FInventoryGrownEvent& OnInventoryGrown() { return inventoryGrownEvent; }

	DECLARE_EVENT_OneParam(UInventoryManagerSubsystem, FInventoryShrankEvent, FOrbID)
	FInventoryShrankEvent& OnInventoryShrank() { return inventoryShrankEvent; }

	DECLARE_EVENT_OneParam(UInventoryManagerSubsystem, FInventoryLoadedEvent, FInventoryStruct)
	FInventoryLoadedEvent& OnInventoryLoaded() { return inventoryLoadedEvent; }

	DECLARE_EVENT_OneParam(UInventoryManagerSubsystem, FShortCutInventoryShrankEvent, int32 )
	FShortCutInventoryShrankEvent& OnShortCutShrank() { return shortCutInventoryShrankEvent; }

	virtual void Initialize(FSubsystemCollectionBase &collection) override;


	UFUNCTION(BlueprintCallable)
	void AddOrb(FOrbDataStruct data);

	UFUNCTION(BlueprintCallable)
	void UseOrb(FOrbDataStruct orbData, bool &orbEmpty);
	void LoadInventory(FInventoryStruct loadedInventory);

	UFUNCTION(BlueprintCallable)
	void MoveFromInventoryToShortcut(FOrbDataStruct data);

	UFUNCTION(BlueprintCallable)
	void MoveFromShortcutToInventory(int32 shortCutID, FOrbDataStruct data);

	virtual TStatId GetStatId() const { return Super::GetStatID(); }
	FInventoryStruct& GetInventoryData() { return inventoryData; }

	UFUNCTION(BlueprintCallable)
	bool CanPlaceInShortCut(FOrbDataStruct data);

	int32 GetOrbAmountInInventory(FOrbID orbID);
	FInventoryStruct& GetInventory() { return inventoryData; }

private:

	void GetOrbsEffectManager();
	void GetDataManagerSubsys();
	void BindDataManagerEvents();
	//Events
	FInventoryGrownEvent inventoryGrownEvent;
	FInventoryShrankEvent inventoryShrankEvent;
	FInventoryLoadedEvent inventoryLoadedEvent;
	FShortCutInventoryShrankEvent shortCutInventoryShrankEvent;
	UDataManagerSubsystem* dataManagerSubsys;

	UOrbsEffectManagerSubsystem* orbsEffectManager;

	FInventoryStruct inventoryData;
	FInventoryStruct shortCutOrbs;
};
