// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/HUDOrbsInfo.h"
#include "Animation/WidgetAnimation.h"
#include "Components/RichTextBlock.h"
#include "Internationalization/Text.h"

#define LOCTEXT_NAMESPACE "OrbsNotifyLocText"
void UHUDOrbsInfo::Notify(FOrbDataStruct orbData)
{
	if (infoText == nullptr) return;

	FText Container = FText::Format(LOCTEXT("Example", " {0} \n {1}"),orbData.shortDescription, orbData.description);
	infoText->SetText(Container);

	ShowAnimation();
}

void UHUDOrbsInfo::Notify(FText notifyTxt)
{
	if (infoText == nullptr) return;

	infoText->SetText(notifyTxt);
	ShowAnimation();
}
#undef LOCTEXT_NAMESPACE
