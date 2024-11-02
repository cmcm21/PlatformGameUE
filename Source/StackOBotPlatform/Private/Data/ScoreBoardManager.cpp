// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ScoreBoardManager.h"
#include "StackOBotPlatform/GameModeData.h"
#include "StackOBotPlatform/GameDefinitions.h"
#include "StackOBotPlatform/Data/GameDataManager.h"

#define LOCTEXT_NAMESPACE "ScoreBoardLocText"

TMap<FString, FScoreDataStruct> UScoreBoardManager::SortByTime(TMap<FString, FScoreDataStruct> data)
{
	data.ValueSort([](const FScoreDataStruct& scoreA, const FScoreDataStruct& scoreB) {
			return scoreA.time < scoreB.time;
		});

	return data;
}

void UScoreBoardManager::IsInRange(TMap<FString, FScoreDataStruct> data, FScoreDataStruct newScore, bool& bOutSuccess, FText &feedback)
{
	if (data.Num() < SCOREBOARD_RANGE)
	{
		bOutSuccess = true;
		return;
	}

	float lastTime = GetLastScoreTime(data, bOutSuccess, feedback);

	if (!bOutSuccess)
		return;

	bOutSuccess = lastTime > newScore.time;
	if (!bOutSuccess)
	{
		feedback = FText::Format(LOCTEXT("ScoreInRangeText", "Score {0} didn't reach scoreboard top"), newScore.time);
	}
}

void UScoreBoardManager::SaveNewScore(TMap<FString, FScoreDataStruct> data, FScoreDataStruct newScore, bool& bOutSuccess, FText& feedback)
{
	FString rowName = FString::Printf(TEXT("Row_%d_%s"), data.Num(), *newScore.gameplayName);
	if (data.Num() + 1 >= SCOREBOARD_RANGE)
	{
		IsInRange(data, newScore,bOutSuccess,feedback);
		if (!bOutSuccess)
			return;

		RemoveLowestScore(data, bOutSuccess, feedback);
		if (!bOutSuccess)
			return;
	}

	if(!IsScoreDuplicated(data,newScore))
		UGameDataManager::WriteScoreToScoreBoard(newScore, data, bOutSuccess, feedback);

	if (bOutSuccess)
		feedback = FText::Format(LOCTEXT("CongratsMsg", "Congrats!!, your score {0} is in scoreboard now"), newScore.time);
}

TMap<FString, FScoreDataStruct> UScoreBoardManager::RemoveLowestScore(TMap<FString, FScoreDataStruct> data, bool& bOutSuccess, 
	FText& feedback)
{
	FString lowestKey = GetLastScoreKey(data, bOutSuccess, feedback);
	if (!bOutSuccess)
		return data;

	int32 beforeRemove = data.Num();
	data.Remove(lowestKey);
	int32 afterRemove = data.Num();

	bOutSuccess = beforeRemove != afterRemove;
	if (!bOutSuccess)
		feedback = FText(LOCTEXT("RemoveLowestScoreFailed", "Remove lowest score failed"));

	return data;
}

FString UScoreBoardManager::GetLastScoreKey(TMap<FString, FScoreDataStruct> data, bool& bOutSuccess, FText& feedback)
{
	if (data.IsEmpty())
	{
		bOutSuccess = false;
		feedback = FText(LOCTEXT("DataEmptyNoLowestKey", "Data is empty, there is no lowest score key"));
		return "";
	}

	data = SortByTime(data);
	int dataNum = data.Num();
	TArray<FString> allKeys;

	data.GetKeys(allKeys);
	bOutSuccess = true;
	feedback = FText(LOCTEXT("GetLowestScore", "get lowest score key success"));

	return allKeys[dataNum - 1];
}

float UScoreBoardManager::GetLastScoreTime(TMap<FString, FScoreDataStruct> data, bool& bOutSuccess, FText& feedback)
{
	if (data.IsEmpty())
	{
		bOutSuccess = false;
		feedback = FText(LOCTEXT("DataIsEmptyNoLowestScore", "Data is empty, there is no lowest score time"));
		return MAX_GAMEPLAY_TIME;
	}

	data = SortByTime(data);
	int dataNum = data.Num();

	TArray<FScoreDataStruct> allValues;
	data.GenerateValueArray(allValues);
	float lowestTime = allValues[dataNum - 1].time;
	bOutSuccess = true;
	feedback = FText(LOCTEXT("GetLowestScoreSuccess", "Get lowest score success"));

	return allValues[dataNum - 1].time;
}

TMap<FString, FScoreDataStruct> UScoreBoardManager::TruncateScoreBoard(TMap<FString, FScoreDataStruct> scoreBoard)
{
	TMap<FString, FScoreDataStruct> truncateScoreBoard;
	for (auto& row : scoreBoard)
	{
		if (truncateScoreBoard.Num() < SCOREBOARD_RANGE)
			truncateScoreBoard.Add(row.Key, row.Value);
		else
			break;
	}
	return truncateScoreBoard;
}

bool UScoreBoardManager::IsScoreDuplicated(TMap<FString, FScoreDataStruct> scoreBoard, FScoreDataStruct newScore)
{
	for (auto& tuple : scoreBoard)
		if (tuple.Value.gameplayName == newScore.gameplayName)
			return true;

	return false;
}

#undef LOCTEXT_NAMESPACE
