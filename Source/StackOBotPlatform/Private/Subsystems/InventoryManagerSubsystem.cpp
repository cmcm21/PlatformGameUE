// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/InventoryManagerSubsystem.h"
#include "Subsystems/OrbsEffectManagerSubsystem.h"
#include "Subsystems/DataManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "StackOBotPlatform/GameModeData.h"

void UInventoryManagerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	inventoryData = FInventoryStruct();
	BindDataManagerEvents();
}

void UInventoryManagerSubsystem::AddOrb(FOrbDataStruct data)
{
    inventoryData.AddOrb(data);
	inventoryGrownEvent.Broadcast(data);
}

void UInventoryManagerSubsystem::UseOrb(FOrbDataStruct data, bool &orbEmpty)
{
	inventoryData.RemoveOrb(data.orbID);

	GetOrbsEffectManager();
	if (orbsEffectManager) orbsEffectManager->ApplyEffect(data);

	orbEmpty = inventoryData.orbs.Find(data.orbID) == nullptr;
	inventoryShrankEvent.Broadcast(data.orbID);
}

void UInventoryManagerSubsystem::LoadInventory(FInventoryStruct loadedInventory)
{
	inventoryLoadedEvent.Broadcast(inventoryData);
	inventoryData = loadedInventory;

	for (auto& inventoryOrbPair : inventoryData.orbs)
		for(int i = 0; i < inventoryData.counter[inventoryOrbPair.Key]; i++)
			inventoryGrownEvent.Broadcast(inventoryOrbPair.Value);
}

void UInventoryManagerSubsystem::MoveFromInventoryToShortcut(FOrbDataStruct data)
{	
	if (inventoryData.orbs.Find(data.orbID) == nullptr) return;
	
	//inventoryData.RemoveOrb(data.orbID, realNumber);
	shortCutOrbs.AddOrb(data,inventoryData.counter[data.orbID]);
}

void UInventoryManagerSubsystem::MoveFromShortcutToInventory(int32 slotID,FOrbDataStruct data)
{
	if (shortCutOrbs.orbs.Find(data.orbID) == nullptr) return;
	
	//inventoryData.AddOrb(data,realNumber);
	inventoryGrownEvent.Broadcast(data);
	shortCutInventoryShrankEvent.Broadcast(slotID);
	shortCutOrbs.RemoveAllOrbs(data.orbID);
}

bool UInventoryManagerSubsystem::CanPlaceInShortCut(FOrbDataStruct data)
{
	return (shortCutOrbs.orbs.Find(data.orbID) == nullptr);
}

int32 UInventoryManagerSubsystem::GetOrbAmountInInventory(FOrbID orbID)
{
	if (inventoryData.orbs.Find(orbID) == nullptr)
		return 0;

	return inventoryData.counter[orbID];
}

void UInventoryManagerSubsystem::GetOrbsEffectManager()
{
	if (orbsEffectManager) return;
	
	UWorld* world = GetWorld();
	if (world) orbsEffectManager = world->GetSubsystem<UOrbsEffectManagerSubsystem>();
}

void UInventoryManagerSubsystem::GetDataManagerSubsys()
{
	if (dataManagerSubsys) return;

	UWorld* world = GetWorld();
	if (world)
	{
		UGameInstance *gameInstance = world->GetGameInstance();
		if (gameInstance)
			dataManagerSubsys = gameInstance->GetSubsystem<UDataManagerSubsystem>();
	}
}

void UInventoryManagerSubsystem::BindDataManagerEvents()
{
	GetDataManagerSubsys();
	if (!dataManagerSubsys) return;

	dataManagerSubsys->OnLoadedEvent().AddLambda([&](FGameplayDataStruct data) {
		LoadInventory(data.inventory);
		});
}
