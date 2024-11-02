// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDOrbEffectState.h"
#include "HUD/HUDOrbEffectStateSingleWidget.h"
#include "Components/VerticalBox.h"
#include "Subsystems/OrbsEffectManagerSubsystem.h"
#include "StackOBotPlatform/Plataformer/PlatformGameMode.h"

void UHUDOrbEffectState::NativeConstruct()
{
	BindOrbsManagerSubsystem();
}

void UHUDOrbEffectState::BindOrbsManagerSubsystem()
{
	GetOrbEffectManagerSubsystem();
	if (!orbsManagerSys)
		return;

	orbsManagerSys->OnStateOrbEffectApplied().AddUObject(this, &UHUDOrbEffectState::OnStateOrbEffectAppliedCallback);
	orbsManagerSys->OnOrbStateTimeUpdated().AddUObject(this, &UHUDOrbEffectState::OnStateOrbEffectUpdateTimeCallback);
	orbsManagerSys->OnOrbStateDiscard().AddUObject(this, &UHUDOrbEffectState::OnStateOrbEffectDiscardCallback);
}

void UHUDOrbEffectState::OnStateOrbEffectAppliedCallback(FOrbID orbEffectID, FOrbDataStruct orbData)
{
	ShowNewOrbEffect(orbEffectID,orbData);
}

void UHUDOrbEffectState::OnStateOrbEffectUpdateTimeCallback(FOrbID orbEffectID, float time)
{
	if (orbEffectWidgetsRefs.Find(orbEffectID))
		orbEffectWidgetsRefs[orbEffectID]->UpdateTimer(time);
}

void UHUDOrbEffectState::OnStateOrbEffectDiscardCallback(FOrbID orbEffectID)
{
	HideOrbState(orbEffectID);
}

void UHUDOrbEffectState::ShowNewOrbEffect(FOrbID orbEffectWidgetID,FOrbDataStruct orbData)
{
	UHUDOrbEffectStateSingleWidget * effectWidget = Cast<UHUDOrbEffectStateSingleWidget>(CreateWidget(GetWorld(), BPWOrbEffectState));
	if (orbEffectWidgetsRefs.Find(orbEffectWidgetID))
	{
		orbEffectWidgetsRefs[orbEffectWidgetID]->SetVisibility(ESlateVisibility::Visible);
	}
	else if (effectWidget)
	{
		effectWidget->InitWidget(orbData);
		effectsList->AddChildToVerticalBox(effectWidget);
		orbEffectWidgetsRefs.Add(orbEffectWidgetID, effectWidget);
	}
}

void UHUDOrbEffectState::HideOrbState(FOrbID orbEffectWidgetID)
{
	if (orbEffectWidgetsRefs.Find(orbEffectWidgetID))
	{
		orbEffectWidgetsRefs[orbEffectWidgetID]->SetVisibility(ESlateVisibility::Hidden);
		//effectsList->RemoveChild(orbEffectWidgetsRefs[orbEffectWidgetID]);
		//orbEffectWidgetsRefs[orbEffectWidgetID]->RemoveFromParent();
		//orbEffectWidgetsRefs.Remove(orbEffectWidgetID);
	}
}

void UHUDOrbEffectState::GetOrbEffectManagerSubsystem()
{
	UWorld* world = GetWorld();

	if (world) orbsManagerSys = world->GetSubsystem<UOrbsEffectManagerSubsystem>();
}

void UHUDOrbEffectState::GetPlatformGameMode()
{
	UWorld* world = GetWorld();
	if (world)
		platformGameMode = Cast<APlatformGameMode>(world->GetAuthGameMode());
}
