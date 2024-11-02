// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveScore.h"
#include "StackOBotPlatform/GameModeData.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

void USaveScore::SetScoreText(float time, int32 falls)
{
	if (scoreDisplay == nullptr) return;

	scoreDisplay->SetText(FText::FromString(FString::Printf(TEXT("Time: %.3f, Falls: %d"), time, falls)));

	gameplayTime = time;
	gameplayFalls = falls;
}

FScoreDataStruct USaveScore::GetGameplayScore(bool &bOutSuccess)
{
	if (playerName == nullptr || (playerName->GetText()).IsEmpty())
	{
		bOutSuccess = false;
		return FScoreDataStruct();
	}

	FScoreDataStruct gameplayStruct = FScoreDataStruct();

	bOutSuccess = true;
    gameplayStruct.gameplayName = (playerName->GetText()).ToString();
    gameplayStruct.falls = gameplayFalls;
    gameplayStruct.time = gameplayTime;
	
	return gameplayStruct;
}

FLevelDataStruct USaveScore::AddGameNameToGameData(FLevelDataStruct data)
{
	data.gameName = (playerName->GetText()).ToString();
	return data;
}


