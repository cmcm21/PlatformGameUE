// Fill out your copyright notice in the Description page of Project Settings.
#include "HUD/PlatformHUD.h"
#include "HUD/HUDTimer.h"
#include "HUD/HUDCollectables.h"
#include "HUD/HUDMenu.h"
#include "HUD/SaveButton.h"
#include "HUD/HUDStaticWidget.h"
#include "HUD/HUDPlayerStats.h"
#include "HUD/HUDOrbsInfo.h"
#include "HUD/HUDSavePoint.h"
#include "UnrealClient.h"

#include "StackOBotPlatform/Plataformer/HUDManager.h"
#include "StackOBotPlatform/Plataformer/PlatformGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/OrbsEffectManagerSubsystem.h"

void APlatformHUD::BeginPlay()
{
	Super::BeginPlay();
	if (BPW_baseHUD)
	{
		hudManager = Cast<UHUDManager>(CreateWidget(GetWorld(), BPW_baseHUD));
		if(hudManager)
			hudManager->AddToViewport();
	}
	BindGameMode();
	BindOrbSubsystem();
}

void APlatformHUD::PushMenuToHUD(HUDMenuID menuID)
{
	if (!hudManager) return;

	if (menuID == HUDMenuID::HUD_SAVE_M)
		hudManager->hudSaveButton->SwitchButtonColor();

	hudManager->PushMenu(menuID);
}

void APlatformHUD::RemoveMenuFromHUD(UHUDMenu *menu)
{
	if (menu->menuID == HUDMenuID::HUD_SAVE_M)
		hudManager->hudSaveButton->SwitchButtonColor();

	if (hudManager)
		hudManager->RemoveMenu(menu);
}

void APlatformHUD::RemoveLastMenu()
{
	if (hudManager)
		hudManager->RemoveLastMenu();
}

void APlatformHUD::ToggleWidget(HUDWidgetID widgetID)
{
	if (hudManager)
		hudManager->ToggleWidget(widgetID);
}

void APlatformHUD::HideWidget(HUDWidgetID widgetID)
{
	if (hudManager)
		hudManager->HideWidget(widgetID);
}

void APlatformHUD::ShowWidget(HUDWidgetID widgetID)
{
	if (hudManager)
		hudManager->ShowWidget(widgetID);
}

void APlatformHUD::ShowCrosshair(bool show)
{
	if (hudManager)
		hudManager->ShowCrosshair(show);
}

FVector2D APlatformHUD::GetGameViewPort()
{
	FVector2D result = FVector2D(1,1);
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(result);

	return result;
}

 void APlatformHUD::BindGameMode()
{
	gameMode = CastChecked<APlatformGameMode>(GetWorld()->GetAuthGameMode());
	
	gameMode->OnTimeUpdatedEvent().AddUObject(this, &APlatformHUD::OnTimeUpdatedCallback);
	gameMode->OnItemCollectedEvent().AddUObject(this, &APlatformHUD::OnCollectablesUpdatedCallback);
	gameMode->OnHpUpdatedEvent().AddUObject(this, &APlatformHUD::OnHPUpdatedCallback);
	gameMode->OnHpMaxUpdatedEvent().AddUObject(this, &APlatformHUD::OnMaxHPUpdatedCallback);
	gameMode->OnExpUpdatedEvent().AddUObject(this, &APlatformHUD::OnExpUpdatedCallback);
	gameMode->OnLevelUpdatedEvent().AddUObject(this, &APlatformHUD::OnUpdateLevelCallback);
	gameMode->OnGameoverEvent().AddUObject(this, &APlatformHUD::OnGameOverCallback);
	gameMode->OnSavePointCheckedEvent().AddUObject(this, &APlatformHUD::OnSavePointCheckCallback);

	if (gameMode->HasActorBegunPlay())
		gameMode->ResendItemsEvent();
}


#define LOCTEXT_NAMESPACE "OrbsNotifyLocText"
void APlatformHUD::BindOrbSubsystem()
{
	UWorld* world = GetWorld();
	if (gameMode == nullptr || gameMode->IsGameOver())
		return;

	if (world)
	{
		UOrbsEffectManagerSubsystem *orbsSubsystem = world->GetSubsystem<UOrbsEffectManagerSubsystem>();

		orbsSubsystem->OnOrbEffectDiscardEvent().AddLambda([&](FOrbDataStruct orb) {
				FText orbName;
				UEnum::GetDisplayValueAsText(orb.orbID, orbName);

				const FText notifyText = 
					FText::Format(LOCTEXT("OrbEffectDiscarted", "<DiscardEffectRow> Orb Effect {0} Discarded </>"), orbName);

				ShowOrbDataNotify(notifyText);
			});

		orbsSubsystem->OnOrbStoredEvent().AddLambda([&](FOrbDataStruct orb) {
				FText orbName;
				UEnum::GetDisplayValueAsText(orb.orbID, orbName);
				const FText notifyText = FText::Format(LOCTEXT("OrbEffectStored", "<StoreEffectRow> Orb {0} Stored </>"), orbName);
				ShowOrbDataNotify(notifyText);
			});

		orbsSubsystem->OnOrbEffectApplied().AddUObject(this, &APlatformHUD::OnOrbAppliedCallback);
	}
}
#undef LOCTEXT_NAMESPACE

void APlatformHUD::OnTimeUpdatedCallback(float seconds)
{
	if (!ValidateGameMode()) return;
	if (!hudManager) return;
	bool success;

	UHUDTimer *timerWidget = 
		Cast<UHUDTimer>(hudManager->GetStaticWidget(HUDWidgetID::HUD_TIME_W, success));

	if (timerWidget)
		timerWidget->UpdateTimer(seconds);
}

void APlatformHUD::OnCollectablesUpdatedCallback(int32 total, int32 collected)
{
	if (!ValidateGameMode()) return;
	if (!hudManager) return;
	bool success;

	UHUDCollectables* collectableWidget = 
		Cast<UHUDCollectables>(hudManager->GetStaticWidget(HUDWidgetID::HUD_ORB_COLLECTION_W, success));

	if (collectableWidget)
		collectableWidget->UpdateItemsText(total, collected);
}


void APlatformHUD::CheckPlayerStats()
{
	if (!hudManager) return;
	bool success;
	
	if(!playerStats)
		playerStats = Cast<UHUDPlayerStats>(hudManager->GetStaticWidget(HUDWidgetID::HUD_PLAYER_STATS_W, success));
}

void APlatformHUD::OnHPUpdatedCallback(int32 maxHP, int32 currentHP)
{
	if (!ValidateGameMode()) return;
	CheckPlayerStats();
	if (playerStats)
		playerStats->UpdateHP(maxHP, currentHP);
}

void APlatformHUD::OnMaxHPUpdatedCallback(int32 maxHP,int32 oldHp ,int32 currentHP)
{
	if (!ValidateGameMode()) return;
	CheckPlayerStats();
	if (playerStats)
		playerStats->UpdateMaxHP(maxHP,oldHp ,currentHP);
}

void APlatformHUD::OnExpUpdatedCallback(int32 nextLevelExp, int32 currentExp)
{
	if (!ValidateGameMode()) return;
	CheckPlayerStats();
	if (playerStats)
		playerStats->UpdateExp(nextLevelExp, currentExp);
}

void APlatformHUD::OnUpdateLevelCallback(int32 updatedLevel)
{
	if (!ValidateGameMode()) return;
	CheckPlayerStats();
	if (playerStats)
		playerStats->UpdateLevelText(updatedLevel);
}

void APlatformHUD::OnGameOverCallback()
{
	FTimerHandle WaitThenPush;
	GetWorldTimerManager().ClearTimer(WaitThenPush);
	GetWorldTimerManager().SetTimer(WaitThenPush, [&]() {PushMenuToHUD(HUDMenuID::GAME_OVER_M); }, 1.5f, false);

	//PushMenuToHUD(HUDMenuID::GAME_OVER_M);
}

void APlatformHUD::OnOrbAppliedCallback(FOrbDataStruct orbData)
{
	if (!hudManager) return;

	UHUDOrbsInfo* info = Cast<UHUDOrbsInfo>(hudManager->CreateHUDWidget(HUDWidgetID::HUD_ORBS_INFO_W));

	if (info)
	{
		FVector2D viewPort = GetGameViewPort();
		float quarterX = viewPort.X / 4;
		FVector2D RandomPosition = FVector2D(FMath::RandRange(viewPort.X/2 - quarterX, viewPort.X - quarterX), 0);

		info->SetPositionInViewport(RandomPosition);
		info->Notify(orbData);
	}
}

void APlatformHUD::OnSavePointCheckCallback()
{
	if (!hudManager) return;

	UHUDSavePoint* savePointInfo = Cast<UHUDSavePoint>(hudManager->CreateHUDWidget(HUDWidgetID::HUD_SAVE_POINT_W));
	if (savePointInfo)
		savePointInfo->Notify();
}

void APlatformHUD::ShowOrbDataNotify(FText notifyText)
{
	if (!hudManager) return;

	UHUDOrbsInfo* info = Cast<UHUDOrbsInfo>(hudManager->CreateHUDWidget(HUDWidgetID::HUD_ORBS_INFO_W));

	if (info)
	{
		FVector2D viewPort = GetGameViewPort();
		float offset = viewPort.X / 3;
		FVector2D RandomPosition = FVector2D(FMath::RandRange(viewPort.X/2 - offset, viewPort.X - offset), 0);

		info->SetPositionInViewport(RandomPosition);
		info->Notify(notifyText);
	}
}

 bool APlatformHUD::ValidateGameMode()
 {
	 return (gameMode != nullptr && !gameMode->IsGameOver());
 }


