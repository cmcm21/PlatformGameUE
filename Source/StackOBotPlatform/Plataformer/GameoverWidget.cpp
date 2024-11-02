// Fill out your copyright notice in the Description page of Project Settings.


#include "GameoverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "UObject/Class.h"

void UGameoverWidget::TriggerResetButtonEvent()
{
	onResetEvent.Broadcast();
}
