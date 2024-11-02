// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDInventory.h"
#include "Components/VerticalBox.h"
#include "Subsystems/InventoryManagerSubsystem.h"

void UHUDInventory::NativeConstruct()
{
	Super::NativeConstruct();
	GetInventoryManagerSys();

	inventorySys->OnInventoryGrown().AddUObject(this, &UHUDInventory::OnInventoryGrown);
	inventorySys->OnInventoryShrank().AddUObject(this, &UHUDInventory::OnInventoryShrank);
	inventorySys->OnInventoryLoaded().AddUObject(this, &UHUDInventory::OnInventoryLoaded);
}

void UHUDInventory::OnInventoryGrown(FOrbDataStruct data)
{
	AddOrb(data);
}

void UHUDInventory::OnInventoryShrank(FOrbID orbID)
{
	RemoveOrb(orbID);
}

void UHUDInventory::OnInventoryLoaded(FInventoryStruct inventoryData)
{
	for (auto& orbPair : inventoryData.orbs)
		AddOrb(orbPair.Value);
}

void UHUDInventory::GetInventoryManagerSys()
{
	UWorld* world = GetWorld();

	if (world) inventorySys = world->GetSubsystem<UInventoryManagerSubsystem>();
}

void UHUDInventory::AddOrb(FOrbDataStruct orbData)
{
	if (inventorySlots.Find(orbData.orbID))
		inventorySlots[orbData.orbID]->Increase();
	else
	{
		auto inventorySlot = CreateInventorySlot();
		if (inventorySlot)
		{
			inventorySlot->InitSlot(orbData);
			inventorySlots.Add(orbData.orbID, inventorySlot);
			AddToGrid(inventorySlot);
		}
	}
}

UHUDInventorySlot *UHUDInventory::CreateInventorySlot()
{
	if (BPWInventorySlot == nullptr) return nullptr;

	UHUDInventorySlot* slot = Cast<UHUDInventorySlot>(CreateWidget(GetWorld(), BPWInventorySlot));
	return slot;
}

void UHUDInventory::AddToGrid(UHUDInventorySlot* slot)
{
	if (inventoryGrid == nullptr) return;

	inventoryGrid->AddChildToVerticalBox(slot);
}

void UHUDInventory::RemoveFromGrid(UHUDInventorySlot* slot)
{
	if (inventoryGrid == nullptr || slot == nullptr) return;

	inventoryGrid->RemoveChild(slot);
}

void UHUDInventory::RemoveOrb(FOrbID orbID)
{
	if (inventorySlots.Find(orbID) == nullptr) return;
	
	inventorySlots[orbID]->Decrease();
	if (inventorySlots[orbID]->GetOrbCounter() <= 0)
	{
		RemoveFromGrid(inventorySlots[orbID]);
		inventorySlots[orbID]->RemoveFromParent();
		inventorySlots.Remove(orbID);
	}
}
