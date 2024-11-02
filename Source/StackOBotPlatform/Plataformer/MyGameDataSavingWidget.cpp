// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameDataSavingWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "StackOBotPlatform/Plataformer/HUDManager.h"
#include "HUD/SaveButton.h"
#include "HUD/PlatformHUD.h"

void UMyGameDataSavingWidget::GoBack()
{
	Super::GoBack();
	if (platformHUD)
	{
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
		pc->bShowMouseCursor = false;

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}
