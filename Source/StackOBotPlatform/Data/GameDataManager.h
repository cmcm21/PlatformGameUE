// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameDataManager.generated.h"

struct FLevelDataStruct;
struct FScoreDataStruct;
struct FSaveSlotMetaData;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UGameDataManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	inline static const FString defaultNewGameFile = "defaultGame.json";
	inline static const FString allGamesFile = "allGames.json";
	inline static const FString allSaveSlotsFile = "allSaveSlots.json";
	inline static const FString scoreBoardFile = "scoreBoard.json";

	UFUNCTION(BlueprintCallable,Category = "GameData Management")
	static void CreateNewGame(FString gameName,FPlayerDataStruct playerData, bool& bOutSuccess, FText& bOutMessage);

	UFUNCTION(BlueprintCallable, Category = "GameData Management")
	static void OverrideGame(FLevelDataStruct dataStruct, bool& bOutSuccess, FText& bOutMessage);

	UFUNCTION(BlueprintCallable, Category="GameData Management")
	static TMap<FString, FLevelDataStruct> OverrideGameRow(FString gameName, FLevelDataStruct newGame, TMap<FString, FLevelDataStruct> data, bool& bOutSuccess, FText& bOutMessage);

	UFUNCTION(BlueprintCallable, Category="GameData Management")
	static void AddNewGameData(FLevelDataStruct dataStruct, TMap<FString,FLevelDataStruct> allGames, bool& bOutSuccess, FText& bOutMessage);

	UFUNCTION(BlueprintCallable,Category="GameData Management")
	static TMap<FString, FLevelDataStruct> GetAllGamesData(bool &bOutSuccess, FText &bOutMessage);

	UFUNCTION(BlueprintCallable,Category = "GameData Management")
	static FLevelDataStruct GetGamedata(bool& bOutSuccess, FText& bOutMessage);

	UFUNCTION(BlueprintCallable, Category = "GameData Management")
	static TMap<FString, FScoreDataStruct> GetScoreboardData(bool &bOutSuccess, FText &bOutMessage);

	UFUNCTION(BlueprintCallable, Category = "GameData Management")
	static void WriteScoreToScoreBoard(FScoreDataStruct score, TMap<FString, FScoreDataStruct> scoreBoard, 
		bool& bOutSuccess, FText& bOutMessage);

	UFUNCTION(BlueprintCallable,Category="PlayerData Management")
	static FPlayerDataStruct GetPlayerDataFromCharacterID(UDataTable *playerDataTable,int32 characterID, bool &bOutSuccess);

	UFUNCTION(BlueprintCallable,Category="SaveSlot Data Management")
	static TMap<FString, FSaveSlotMetaData> GetAllSlots(bool &boutSuccess, FText &bOutMessage);

	UFUNCTION(BlueprintCallable,Category="SaveSlot Data Management")
	static void SaveSlot(FSaveSlotMetaData slotMetaData, bool &bOutSuccess, FText &bOutMessage);

	UFUNCTION(BlueprintPure,Category = "GameData Management")
	static FString GetDefaultNewGamePath();

	UFUNCTION(BlueprintPure,Category = "GameData Management")
	static FString GetScoreBoardPath();

	UFUNCTION(BlueprintPure,Category = "GameData Management")
	static FString GetAllGamesPath();

	UFUNCTION(BlueprintPure, Category="SaveSlot Data Management")
	static FString GetSaveSlotDataPath();

};
