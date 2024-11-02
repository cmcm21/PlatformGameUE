// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StackOBotPlatform/GameModeData.h"
#include "DataManagerSubsystem.generated.h"

class USaveGame;
class APlatformGameMode;
struct FGameplayDataStruct;
struct FSaveSlotMetaData;
class UOrbsEffectManagerSubsystem;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UDataManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	DECLARE_EVENT_OneParam(UDataManagerSubsystem, DataLoadedEvent, FGameplayDataStruct)
	DataLoadedEvent& OnLoadedEvent() { return loadedEvent; }

	DECLARE_EVENT_OneParam(UDataManagerSubSystem, DataSavedEvent, bool)
	DataSavedEvent& OnGameSaveEvent() { return savedEvent; }

	virtual void Initialize(FSubsystemCollectionBase &collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintReadWrite, Category="SaveSlot Data Management")
	FSaveSlotMetaData selectedSlot;

	UPROPERTY(BlueprintReadWrite, Category="Data Management")
	FGameplayDataStruct defaultGame;

	UFUNCTION(BlueprintCallable)
	bool AsyncLoadGameplayData(FString saveSlot);

	UFUNCTION(BlueprintCallable)
	void SaveCurrentGameplay();

	UFUNCTION(BlueprintCallable)
	void SaveGameplay(FGameplayDataStruct dataStruct);

	UFUNCTION(BlueprintCallable)
	void OverrideGameplay(FString saveSlot);

	UFUNCTION(BlueprintCallable)
	void CreateNewGame(FString slotName, FPlayerDataStruct playerstats);

	UFUNCTION(BlueprintCallable)
	void SetSelectedSlot(FSaveSlotMetaData slot);

private:
	DataLoadedEvent loadedEvent;
	DataSavedEvent savedEvent;
	APlatformGameMode *gameMode;

	void SaveGameCallback(const FString& slotName, const int32 userIndex, bool bSuccess);
	void LoadGameCallback(const FString& slotName, const int32 userIndex, USaveGame *loadedGameData);
	void SaveCurrentGameplay(FString saveSlot);
	FString GetNewName(FString source);
	void GetGameMode();
};
