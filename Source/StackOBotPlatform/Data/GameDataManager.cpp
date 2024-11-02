// Fill out your copyright notice in the Description page of Project Settings.
#include "GameDataManager.h"
#include "StackOBotPlatform/GameModeData.h"
#include "Misc/Paths.h"
#include "FileManagement.h"
#include "JsonManagement.h"
#include "StackOBotPlatform/Public/Data/ScoreBoardManager.h"
#include "StackOBotPlatform/GameDefinitions.h"

#define LOCTEXT_NAMESPACE "GameDataManagerLocText"

void UGameDataManager::CreateNewGame(FString gameName,FPlayerDataStruct playerData,bool &bOutSuccess, FText &bOutMessage)
{
	FLevelDataStruct gameStruct = FLevelDataStruct();
	gameStruct.playerData = playerData;
	gameStruct.playerData.characterName = gameName;
	gameStruct.gameName = gameName;

	UJsonManagement::WriteStructToJsonFile<FLevelDataStruct>(
		UGameDataManager::GetDefaultNewGamePath(), gameStruct, bOutSuccess, bOutMessage);
}

void UGameDataManager::OverrideGame(FLevelDataStruct dataStruct, bool& bOutSuccess, FText& bOutMessage)
{
	UJsonManagement::WriteStructToJsonFile<FLevelDataStruct>(UGameDataManager::GetDefaultNewGamePath(), dataStruct, bOutSuccess, bOutMessage);
}

TMap<FString,FLevelDataStruct> UGameDataManager::OverrideGameRow(FString gameName, FLevelDataStruct newGame, TMap<FString, FLevelDataStruct> data, bool& bOutSuccess, FText& bOutMessage)
{
	TArray<FString> dataKeys;
	data.GetKeys(dataKeys);
	bOutSuccess = false;

	for (FString dataKey : dataKeys)
	{
		if(data[dataKey].gameName.Equals(gameName))
		{
			newGame.gameName = gameName;
			data[dataKey] = newGame;
			bOutSuccess = true;
			break;
		}
	}

	if (!bOutSuccess)
	{
		bOutMessage = FText::Format(LOCTEXT("OverrideGameLoc", "No game data found with game name : {0}"), FText::FromString(gameName));
		return data;
	}

	UJsonManagement::SerializeMapToJson(GetAllGamesPath(), data, bOutSuccess, bOutMessage);
	return data;
}


FLevelDataStruct UGameDataManager::GetGamedata(bool &bOutSuccess, FText &bOutMessage)
{
	return UJsonManagement::ReadFromJsonFileToStruct<FLevelDataStruct>(GetDefaultNewGamePath(), bOutSuccess, bOutMessage);
}

void UGameDataManager::AddNewGameData(FLevelDataStruct gameData,TMap<FString,FLevelDataStruct> allGamesData ,
	bool& bOutSuccess, FText& bOutMessage)
{
	FString rowName = FString::Printf(TEXT("Row_%d_%s"), allGamesData.Num(), *gameData.gameName);

	allGamesData.Add(rowName, gameData);
	UJsonManagement::SerializeMapToJson<FLevelDataStruct>(GetAllGamesPath(), allGamesData, bOutSuccess, bOutMessage);
}

TMap<FString, FLevelDataStruct> UGameDataManager::GetAllGamesData(bool& bOutSuccess, FText& bOutMessage)
{
	return UJsonManagement::DeserializeJsonToMap<FLevelDataStruct>(GetAllGamesPath(), bOutSuccess, bOutMessage);
}

TMap<FString, FScoreDataStruct> UGameDataManager::GetScoreboardData(bool &bOutSuccess, FText &bOutMessage)
{
	TMap<FString,FScoreDataStruct> scoreBoard = 
		UJsonManagement::DeserializeJsonToMap<FScoreDataStruct>(GetScoreBoardPath(), bOutSuccess, bOutMessage);

	scoreBoard = UScoreBoardManager::SortByTime(scoreBoard);

	if (scoreBoard.Num() > SCOREBOARD_RANGE)
		return UScoreBoardManager::TruncateScoreBoard(scoreBoard);

	return scoreBoard;
}

void UGameDataManager::WriteScoreToScoreBoard(FScoreDataStruct score, TMap<FString, FScoreDataStruct> scoreBoard, 
	bool& bOutSuccess, FText& bOutMessage)
{
	FString rowName = FString::Printf(TEXT("Row_%d_%s"), scoreBoard.Num(), *score.gameplayName);
	scoreBoard.Add(rowName, score);
	scoreBoard = UScoreBoardManager::SortByTime(scoreBoard);

	UJsonManagement::SerializeMapToJson<FScoreDataStruct>(GetScoreBoardPath(), scoreBoard, bOutSuccess, bOutMessage);
}

FPlayerDataStruct UGameDataManager::GetPlayerDataFromCharacterID(UDataTable* playerDataTable, int32 characterID,bool &bOutSuccess)
{
	TArray<FPlayerDataStruct *> allPlayerData;

	playerDataTable->GetAllRows<FPlayerDataStruct>(FString::Printf(TEXT("Simple Context")), allPlayerData);
	for (FPlayerDataStruct* playerRow : allPlayerData)
	{
		if (playerRow->characterID == characterID)
		{
			bOutSuccess = true;
			return *playerRow;
		}
	}

	bOutSuccess = false;
	return FPlayerDataStruct();
}

TMap<FString,FSaveSlotMetaData> UGameDataManager::GetAllSlots(bool &bOutSuccess, FText &bOutMessage)
{
	return UJsonManagement::DeserializeJsonToMap<FSaveSlotMetaData>(
		GetSaveSlotDataPath(),bOutSuccess,bOutMessage);
}

void UGameDataManager::SaveSlot(FSaveSlotMetaData slotMetaData, bool& bOutSuccess, FText& bOutMessage)
{
	TMap<FString,FSaveSlotMetaData> allSlots = GetAllSlots(bOutSuccess, bOutMessage);
	//if bOutSuccess is false then the file doesn't exist or is empty, so we can continue just adding the new slot
	//and create a new json file 
	FString rowName = FString::Printf(TEXT("row_%d"), allSlots.Num());
	allSlots.Add(rowName, slotMetaData);

	UJsonManagement::SerializeMapToJson<FSaveSlotMetaData>(
		GetSaveSlotDataPath(), allSlots, bOutSuccess, bOutMessage);
}

FString UGameDataManager::GetDefaultNewGamePath()
{
	return FString::Printf(TEXT("%s%s"),*(FPaths::ProjectSavedDir()), *defaultNewGameFile);
}

FString UGameDataManager::GetScoreBoardPath()
{
	return FString::Printf(TEXT("%s%s"), *(FPaths::ProjectSavedDir()), *scoreBoardFile);
}

FString UGameDataManager::GetAllGamesPath()
{
	return FString::Printf(TEXT("%s%s"), *(FPaths::ProjectSavedDir()), *allGamesFile);
}

FString UGameDataManager::GetSaveSlotDataPath()
{
	return FString::Printf(TEXT("%s%s"), *(FPaths::ProjectSavedDir()), *allSaveSlotsFile);
}

#undef LOCTEXT_NAMESPACE
