// Fill out your copyright notice in the Description page of Project Settings.
#include "Subsystems/TimeFormatingSystem.h"
#include "Misc/Timespan.h"
#include "Kismet/KismetStringLibrary.h"


FString UTimeFormatingSystem::GetJustSecondsString(float seconds)
{
	GetSplitTime(seconds);
	return stringSeconds;
}

FString UTimeFormatingSystem::GetJustMilisecondsString(float seconds)
{
	GetSplitTime(seconds);
	return stringMiliseconds;
}

FString UTimeFormatingSystem::GetJustMinutesString(float seconds)
{
	GetSplitTime(seconds);
	return stringMinutes;
}

FString UTimeFormatingSystem::GetTimeString(float seconds)
{
	return UKismetStringLibrary::TimeSecondsToString(seconds);
}

void UTimeFormatingSystem::GetSplitTime(float seconds)
{
	FString baseString = UKismetStringLibrary::TimeSecondsToString(seconds);
	FString secondsAndMiliseconds;
	UKismetStringLibrary::Split(baseString, TEXT(":"), stringMinutes, secondsAndMiliseconds);
	if (seconds < 600) stringMinutes.RemoveAt(0);

	UKismetStringLibrary::Split(secondsAndMiliseconds, TEXT("."), stringSeconds, stringMiliseconds);
	if (seconds < 10) stringSeconds.RemoveAt(0);
}
