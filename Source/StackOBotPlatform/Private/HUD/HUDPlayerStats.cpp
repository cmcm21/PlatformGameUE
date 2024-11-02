// Fill out your copyright notice in the Description page of Project Settings.
#include "HUD/HUDPlayerStats.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "Misc/App.h"

bool UHUDPlayerStats::Initialize()
{
	world = GetWorld();
	return Super::Initialize();
}
void UHUDPlayerStats::SetHpPercentage(int32 totalHP, int32 currentHP)
{
	check(hpProgressBar);
	float percentage = (float)currentHP / (float)totalHP;
	hpProgressBar->SetPercent(percentage);
}

float UHUDPlayerStats::GetIncreasePadding(float currentPadding, int32 newMaxHp, int32 oldHp)
{
	float currentPercentage = hpProgressBar->GetPercent();
	float newPercentage = (float) oldHp/ (float)newMaxHp;
	float difference = currentPercentage - newPercentage;

	return (currentPadding * difference);
}

void UHUDPlayerStats::UpdateHP(int32 totalHP, int32 currentHP)
{
	AnimateHP(totalHP, currentHP);
}

void UHUDPlayerStats::UpdateExp(int32 nextLevelExp, int32 currentExp)
{
	AnimateExp(nextLevelExp, currentExp);
}

void UHUDPlayerStats::UpdateLevelText(int32 level)
{
	if (isExpAnimationPlaying)
	{
		shouldUpdateLevel = true;
		updatedLevel = level;
	}
	else 
		SetLevelText(level);
}

void UHUDPlayerStats::AnimateHP(int32 totalHp, int32 currentHp)
{
	hpPercentageGoal = 0;
	hpPercentageGoal = ((float)currentHp) / ((float)totalHp);
	hpTimer = 0;

	GetWorld()->GetTimerManager().SetTimer(hpTimerHandle, this, &UHUDPlayerStats::OnHPTimerCallback, animationSpeed, true);
}

void UHUDPlayerStats::AnimateExp(int32 nextLevelExp, int32 currentExp)
{
	float expPercentageGoal = ((float)currentExp)/ ((float)nextLevelExp);
	
	if (isExpAnimationPlaying)
		expAnimationsQueue.Add(expPercentageGoal);
	else
		TriggerExpAnimationTimer(expPercentageGoal);
}

void UHUDPlayerStats::TriggerExpAnimationTimer(float expPercentageGoal)
{
	isExpAnimationPlaying = true;
	expTimer = 0;

	GetWorld()->GetTimerManager().SetTimer( expTimerHandle,[&,expPercentageGoal] {
			OnExpTimerCallback(expPercentageGoal);
		}, animationSpeed, true );
}

void UHUDPlayerStats::OnHPTimerCallback()
{
	hpTimer += FApp::GetDeltaTime();
	if (hpTimer <= hpAnimationDuration)
	{
		float currentValue = hpProgressBar->GetPercent();
		float newValue = FMath::Lerp<float>(currentValue, hpPercentageGoal, hpTimer / hpAnimationDuration);

		hpProgressBar->SetPercent(newValue);
	}
	else 
	{
		hpTimer = 0;
		hpProgressBar->SetPercent(hpPercentageGoal);

		if(world) world->GetTimerManager().ClearTimer(hpTimerHandle);
	}
}

void UHUDPlayerStats::OnExpTimerCallback(float expPercentageGoal)
{
	if (this == nullptr || expProgressBar == nullptr || world == nullptr) return;
	expTimer += FApp::GetDeltaTime();

	if (expPercentageGoal == 0)
		expTimer += expAnimationDuration / 2;

	if (expTimer <= expAnimationDuration)
	{
		float currentValue = expProgressBar->GetPercent();
		float newValue = FMath::Lerp<float>(currentValue, expPercentageGoal, expTimer / expAnimationDuration);

		expProgressBar->SetPercent(newValue);
	}
	else 
	{
		expTimer = 0;
		expProgressBar->SetPercent(expPercentageGoal);

		if (world != nullptr)
		{
			world->GetTimerManager().ClearTimer(expTimerHandle);
		}

		if (shouldUpdateLevel)
		{
			SetLevelText(updatedLevel);
			shouldUpdateLevel = false;
		}

		if (expAnimationsQueue.Num() > 0)
		{
			float nextAnimGoal = expAnimationsQueue[0];
			expAnimationsQueue.RemoveAt(0);
			TriggerExpAnimationTimer(nextAnimGoal);
		}
		else
			isExpAnimationPlaying = false;
	}
}

void UHUDPlayerStats::SetLevelText(int32 level)
{
	if (levelText)
		levelText->SetText(FText::FromString(FString::Printf(TEXT("Lvl.%d "), level)));
}

