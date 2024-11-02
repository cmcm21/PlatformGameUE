// Fill out your copyright notice in the Description page of Project Settings.
#include "HUDManager.h"
#include "HUD/IHUDWidget.h"
#include "HUD/HUDMenu.h"
#include "HUD/HUDStaticWidget.h"
#include "HUD/SaveButton.h"
#include "HUD/HUDTimer.h"
#include "HUD/HUDOrbsInfo.h"
#include "HUD/HUDPlayerStats.h"
#include "HUD/HUDCollectables.h"
#include "HUD/HUDToggleableWidget.h"
#include "HUD/HUDOrbEffectState.h"
#include "HUD/HUDInventory.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Containers/Map.h"
#include "Components/Image.h"

bool UHUDManager::Initialize()
{
	//we need to call Super::Initialize before try to access variables of the class
	bool initializeValue = Super::Initialize();
	UWorld* world = GetWorld();

	BuildWidgetsReferences();
	return initializeValue;
}

UHUDStaticWidget* UHUDManager::GetStaticWidget(HUDWidgetID widgetID, bool &bOutSucess)
{
	UHUDStaticWidget** findPtr = staticWidgets.Find(widgetID);
	if (findPtr == nullptr)
	{
		bOutSucess = false;
		return nullptr;
	}

	bOutSucess = true;
	return *findPtr;
}

TSubclassOf<class UHUDMenu> UHUDManager::GetMenuWidgetClass(HUDMenuID menuID, bool& bOutSuccess)
{
	auto classRef = BPWmenusRefs.Find(menuID);
	if (classRef == nullptr)
	{
		bOutSuccess = false;
		return TSubclassOf<class UHUDMenu>();
	}

	bOutSuccess = true;
	return *classRef;
}

bool UHUDManager::ToggleWidget(HUDWidgetID widgetID)
{
	UHUDToggleableWidget **findPtr = toggleWidgets.Find(widgetID);
	if (findPtr == nullptr)
		return false;

	toggleWidgets[widgetID]->Toggle();
	return true;
}

UIHUDWidget* UHUDManager::CreateHUDWidget(HUDWidgetID widgetID)
{
	auto BPW = GetBPW(widgetID);
	if(BPW != nullptr)
	{
		UIHUDWidget *ptr = Cast<UIHUDWidget>(CreateWidget(GetWorld(), BPW));
		ptr->AddToViewport();
		return ptr;
	}
	return nullptr;
}

void UHUDManager::HideWidget(HUDWidgetID widgetID)
{
	if(!staticWidgets.Find(widgetID)) return;

	staticWidgets[widgetID]->Hide();
}

void UHUDManager::ShowWidget(HUDWidgetID widgetID)
{
	if(!staticWidgets.Find(widgetID)) return;

	staticWidgets[widgetID]->Show();
}

void UHUDManager::BuildWidgetsReferences()
{
	//Static Widgets
	staticWidgets.Add(HUDWidgetID::HUD_SAVE_W, hudSaveButton);
	staticWidgets.Add(HUDWidgetID::HUD_TIME_W, hudTimer);
	staticWidgets.Add(HUDWidgetID::HUD_PLAYER_STATS_W, hudPlayerStats);
	staticWidgets.Add(HUDWidgetID::HUD_ORB_COLLECTION_W, hudCollectables);
	staticWidgets.Add(HUDWidgetID::HUD_ORB_EFFECT_STATE_W_LIST, hudOrbStateList);

	//toggables Widgets
	toggleWidgets.Add(HUDWidgetID::HUD_INVENTOR_W, hudInventory);
	toggleWidgets.Add(HUDWidgetID::HUD_DIED_W,hudDead);
}

TSubclassOf<class UIHUDWidget> UHUDManager::GetBPW(HUDWidgetID widgetID)
{
	if (widgetID == HUDWidgetID::HUD_SAVE_POINT_W)
		return (BPWHudSavePoint);
	else if (widgetID == HUDWidgetID::HUD_ORBS_INFO_W)
		return (BPWHudOrbsInfo);
	else
		return nullptr;
}

void UHUDManager::ShowCrosshair(bool show)
{
	if(CrosshairImag)
		CrosshairImag->SetVisibility(show ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
