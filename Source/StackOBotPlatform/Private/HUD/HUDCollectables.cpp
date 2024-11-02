// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDCollectables.h"
#include "Components/RichTextBlock.h"

void UHUDCollectables::UpdateItemsText(int32 totalItems, int32 itemsCollected)
{
	if (itemsText == nullptr) return;

	itemsText->SetText(
		FText::Format(FTextFormat::FromString(
			TEXT("<NormalOrbRow>{0}</><NormalOrbRow>/{1}</>")),itemsCollected,totalItems));
}

