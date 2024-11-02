// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDMenu.h"
#include "HUD/PlatformHUD.h"
#include "StackOBotPlatform/Plataformer/HUDManager.h"

void UHUDMenu::GoBack()
{
	if (platformHUD)
		platformHUD->RemoveMenuFromHUD(this);
}

void UHUDMenu::NativeOnActivated()
{
	Super::NativeOnActivated();
	platformHUD = CastChecked<APlatformHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UHUDMenu::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}
