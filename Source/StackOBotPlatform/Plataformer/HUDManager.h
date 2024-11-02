// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StackOBotPlatform/GameModeData.h"
#include "HUDManager.generated.h"

class UCommonActivatableWidgetStack;
class UUserWidget;
class USaveButton;
class UHUDMenu;
class UHUDStaticWidget;
class UHUDCollectables;
class UHUDToggleableWidget;
class UHUDTimer;
class UHUDPlayerStats;
class UHUDOrbsInfo;
class UIHUDWidget;
class UHUDOrbEffectState;
class UHUDInventory;
class FCanvasTitleItem;
class UImage;


UCLASS()
class STACKOBOTPLATFORM_API UHUDManager : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void ShowCrosshair(bool show);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<HUDMenuID,TSubclassOf<class UHUDMenu>> BPWmenusRefs;

/////////////////////Bind Widgets References//////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="HUDManager")
	UHUDTimer* hudTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="HUDManager")
	USaveButton* hudSaveButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="HUDManager")
	UHUDCollectables* hudCollectables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="HUDManager")
	UHUDPlayerStats* hudPlayerStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="HUDManager")
	UHUDOrbEffectState* hudOrbStateList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="HUDManager")
	UHUDInventory* hudInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="HUDManager")
	UHUDToggleableWidget* hudDead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category = "Crosshair")
	UImage *CrosshairImag;
/////////////////////////////////////////////////////////////////////


////////////////////Notify Widgets//////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UIHUDWidget> BPWHudOrbsInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UIHUDWidget> BPWHudSavePoint;
//////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable)
	UHUDStaticWidget* GetStaticWidget(HUDWidgetID widgetID, bool &bOutSuccess);

	UFUNCTION(BlueprintPure)
	TSubclassOf<class UHUDMenu> GetMenuWidgetClass(HUDMenuID menuID, bool& bOutSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category="Stack Widgets Management")
	void PushMenu(HUDMenuID menuID);

	UFUNCTION(BlueprintImplementableEvent, Category="Stack Widgets Management")
	void RemoveMenu(UHUDMenu *menu);

	UFUNCTION(BlueprintImplementableEvent, Category="Stack Widgets Management")
	void RemoveLastMenu();

	UFUNCTION(BlueprintCallable)
	bool ToggleWidget(HUDWidgetID widgetID);

	UIHUDWidget* CreateHUDWidget(HUDWidgetID widgetID);

	void HideWidget(HUDWidgetID widgetID);

	void ShowWidget(HUDWidgetID widgetID);

protected:

	TMap <HUDWidgetID, UHUDToggleableWidget*> toggleWidgets;
	TMap <HUDWidgetID, UHUDStaticWidget*> staticWidgets;
	FCanvasTitleItem *tileItem;

private:
	UHUDToggleableWidget *activeToggleWidget;
	void BuildWidgetsReferences();
	TSubclassOf<class UIHUDWidget>  GetBPW(HUDWidgetID);
};
