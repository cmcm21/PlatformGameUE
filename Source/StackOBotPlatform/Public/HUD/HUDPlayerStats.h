// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUDStaticWidget.h"
#include "HUDPlayerStats.generated.h"

class UProgressBar;
class UTextBlock;
using namespace UE::Tasks;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UHUDPlayerStats : public UHUDStaticWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar *hpProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* expProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float animationSpeed = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float hpAnimationDuration = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float expAnimationDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextBlock* levelText;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMaxHP(int32 newMax,int32 oldHp,int32 currentHP);

	UFUNCTION(BlueprintCallable)
	void SetHpPercentage(int32 totalHP, int32 currentHP);

	UFUNCTION(BlueprintPure)
	float GetIncreasePadding(float currentPadding, int32 newMaxHp, int32 oldHp);

	UFUNCTION(BlueprintCallable)
	void UpdateHP(int32 totalHP, int32 currentHP);
	void UpdateExp(int32 nextLevelExp, int32 currentExp);
	void UpdateLevelText(int32 level);

protected:
	virtual bool Initialize() override;

private:
	FTimerHandle hpTimerHandle;
	FTimerHandle expTimerHandle;
	class UWorld* world;
	TArray<float> expAnimationsQueue;

	float hpPercentageGoal;
	float hpTimer;
	int newLevel;

	float expTimer;
	bool isExpAnimationPlaying = false;
	bool shouldUpdateLevel = false;
	int updatedLevel;
	
	void AnimateHP(int32 totalHp, int32 currentHP);
	void AnimateExp(int32 nextLevelExp, int32 currentExp);

	void TriggerExpAnimationTimer(float expPercentageGoal);
	void OnHPTimerCallback();
	void OnExpTimerCallback(float expPercentageGoal);
	void SetLevelText(int32 level);
};
