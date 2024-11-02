// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/HUDTimer.h"
#include "StackOBotPlatform/Public/Subsystems/TimeFormatingSystem.h"
#include "Components/RichTextBlock.h"
#include "Kismet/KismetStringLibrary.h" //UKismetStringLibrary

void UHUDTimer::UpdateTimer(float time)
{
	if (timerText == nullptr || GEngine == nullptr) return;
	FString timerStringFormat;
	FString timeString = UKismetStringLibrary::TimeSecondsToString(time);

	timerStringFormat.Append(FString::Printf(TEXT("<minutes>%ss</>"), *timeString));
	//FString miliseconds = GEngine->GetEngineSubsystem<UTimeFormatingSystem>()->GetJustMilisecondsString(time);
	//FString seconds = GEngine->GetEngineSubsystem<UTimeFormatingSystem>()->GetJustSecondsString(time);
	// FString minutes = GEngine->GetEngineSubsystem<UTimeFormatingSystem>()->GetJustMinutesString(time);

	//if(time >= 60.0f)
	//timerStringFormat.Append(FString::Printf(TEXT("<minutes>%s:</>"), *minutes));
	//if(time >= 1.0f)
	//timerStringFormat.Append(FString::Printf(TEXT("<seconds>%s</>"), *seconds));

	//timerStringFormat.Append(FString::Printf(TEXT("<miliseconds>.%ss</>"), *miliseconds));
	
	timerText->SetText(FText::FromString(timerStringFormat));
}
