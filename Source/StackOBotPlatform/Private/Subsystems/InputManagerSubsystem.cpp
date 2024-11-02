// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/InputManagerSubsystem.h"

void UInputManagerSubsystem::TriggerShortCutEvent(int32 keyId)
{
	shortCutEvent.Broadcast(keyId);
}
