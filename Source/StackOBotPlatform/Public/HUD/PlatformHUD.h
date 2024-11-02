// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlatformHUD.generated.h"

class UHUDManager;
class UUserWidget;
class APlatformGameMode;
class UHUDPlayerStats;
struct FOrbDataStruct;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API APlatformHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SaveGameHUD(HUDMenuID ID);

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> BPW_baseHUD;

	UFUNCTION(BlueprintCallable)
	void PushMenuToHUD(HUDMenuID menuID);

	UFUNCTION(BlueprintCallable)
	void RemoveMenuFromHUD(UHUDMenu *menu);

	UFUNCTION(BlueprintCallable)
	void RemoveLastMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleWidget(HUDWidgetID widgetID);

	UFUNCTION()
	void HideWidget(HUDWidgetID widgetID);

	UFUNCTION()
	void ShowWidget(HUDWidgetID widgetID);

	UFUNCTION()
	void ShowCrosshair(bool show);

private:
	APlatformGameMode* gameMode;
	UHUDManager* hudManager;
	UHUDPlayerStats* playerStats;

	FVector2D GetGameViewPort();
	void BindGameMode();
	bool ValidateGameMode();
	void BindOrbSubsystem();
	void CheckPlayerStats();
	void OnTimeUpdatedCallback(float seconds);
	void OnCollectablesUpdatedCallback(int32 total, int32 collected);
	void OnHPUpdatedCallback(int32 maxHP, int32 currentHp);
	void OnMaxHPUpdatedCallback(int32 maxHP,int32 oldMaxHP ,int32 currentHp);
	void OnExpUpdatedCallback(int32 nextLevelExp, int32 currentHp);
	void OnUpdateLevelCallback(int32 updatedLevel);
	void OnGameOverCallback();
	void OnOrbAppliedCallback(FOrbDataStruct orbData);
	void OnSavePointCheckCallback();
	void ShowOrbDataNotify(FText notifyText);
};
