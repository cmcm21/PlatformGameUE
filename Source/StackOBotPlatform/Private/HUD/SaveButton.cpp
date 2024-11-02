// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SaveButton.h"
#include "StackOBotPlatform/Plataformer/HUDManager.h"
#include "Components/Button.h"

USaveButton::USaveButton()
{
}

bool USaveButton::Initialize()
{
	bool returnValue = Super::Initialize();
	return returnValue;
}

void USaveButton::Hide()
{
	SetRenderOpacity(0);
	//saveButton->SetColorAndOpacity(FLinearColor(0, 0, 0, 0));
}

void USaveButton::Show()
{
	SetRenderOpacity(1);
	//saveButton->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
}
