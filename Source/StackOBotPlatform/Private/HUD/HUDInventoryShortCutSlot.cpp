// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDInventoryShortCutSlot.h"
#include "Subsystems/InputManagerSubsystem.h"
#include "Subsystems/InventoryManagerSubsystem.h"

void UHUDInventoryShortCutSlot::NativeConstruct()
{
	Super::NativeConstruct();

	GetInputManagerSubsys();
	inputManagerSubsys->OnShortCutPressed().AddUObject(this, &UHUDInventoryShortCutSlot::OnShortCutPressed);

	GetInventoryManagerSubsys();
	inventoryManagerSubsys->OnShortCutShrank().AddUObject(this, &UHUDInventoryShortCutSlot::OnItemMovedToInventory);
	inventoryManagerSubsys->OnInventoryShrank().AddUObject(this, &UHUDInventoryShortCutSlot::OnInventoryUpdatedCallback);
}

void UHUDInventoryShortCutSlot::GetInputManagerSubsys()
{
	UWorld* world = GetWorld();
	if (world)
	{
		UGameInstance* gameInstance = world->GetGameInstance();
		if (gameInstance)
			inputManagerSubsys = gameInstance->GetSubsystem<UInputManagerSubsystem>();
	}
}

void UHUDInventoryShortCutSlot::GetInventoryManagerSubsys()
{
	UWorld* world = GetWorld();
	if (world)
		inventoryManagerSubsys = world->GetSubsystem<UInventoryManagerSubsystem>();
}

void UHUDInventoryShortCutSlot::OnInventoryUpdatedCallback(FOrbID orbID)
{
	if (slotOrbData.orbID == orbID)
	{
		int32 amount = inventoryManagerSubsys->GetOrbAmountInInventory(orbID);
		if (amount > 0)
			OnInventoryUpdated(amount);
		else
			ClearSlot();
	}
}
