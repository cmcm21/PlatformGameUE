// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDStaticWidget.h"
#include "StackOBotPlatform/GameModeData.h"
#include "HUDOrbEffectState.generated.h"

class URichTextBlock;
class UTexture;
class UHUDOrbEffectStateSingleWidget;
class UListView;
class UVerticalBox;
class UOrbsEffectManagerSubsystem;
class APlatformGameMode;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDOrbEffectState : public UHUDStaticWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void ShowNewOrbEffect(FOrbID orbWidgetID,FOrbDataStruct orbData);

	UFUNCTION(BlueprintCallable)
	void HideOrbState(FOrbID orbEffectWidgetID);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UHUDOrbEffectStateSingleWidget> BPWOrbEffectState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UVerticalBox *effectsList;

private:

	UOrbsEffectManagerSubsystem	 *orbsManagerSys;
	TMap<FOrbID,UHUDOrbEffectStateSingleWidget *> orbEffectWidgetsRefs;
	APlatformGameMode* platformGameMode;

	void GetOrbEffectManagerSubsystem();
	void GetPlatformGameMode();
	void BindOrbsManagerSubsystem();
	void OnStateOrbEffectAppliedCallback(FOrbID orbEffectID, FOrbDataStruct orbData);
	void OnStateOrbEffectUpdateTimeCallback(FOrbID orbEffectID, float time);
	void OnStateOrbEffectDiscardCallback(FOrbID orbEffectID);
};
