// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDInventorySlot.h"
#include "Subsystems/InventoryManagerSubsystem.h"

void UHUDInventorySlot::UseOrb()
{
	GetInventoryManager();

	bool orbEmtpy = false;
	if (inventoryManager )
		inventoryManager->UseOrb(slotOrbData,orbEmtpy);
}

void UHUDInventorySlot::GetInventoryManager()
{
	if (inventoryManager) return;

	UWorld* world = GetWorld();
	if (world)
		inventoryManager = world->GetSubsystem<UInventoryManagerSubsystem>();
}
