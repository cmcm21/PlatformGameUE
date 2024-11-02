// Fill out your copyright notice in the Description page of Project Settings.
#include "Subsystems/DataManagerSubsystem.h"
#include "StackOBotPlatform/Public/Data/GameDataSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "StackOBotPlatform/Plataformer/PlatformGameMode.h"
#include "StackOBotPlatform/Character/PlatformCharacter.h"
#include "StackOBotPlatform/GameModeData.h"
#include "Subsystems/OrbsEffectManagerSubsystem.h"

void UDataManagerSubsystem::Initialize(FSubsystemCollectionBase &collection)
{
	Super::Initialize(collection);
}

void UDataManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDataManagerSubsystem::AsyncLoadGameplayData(FString saveSlot)
{
	FAsyncLoadGameFromSlotDelegate loadedDelegate;
	loadedDelegate.BindUObject(this, &UDataManagerSubsystem::LoadGameCallback);

	if (!UGameplayStatics::DoesSaveGameExist(saveSlot, 0))
		return false;

	UGameplayStatics::AsyncLoadGameFromSlot(saveSlot, 0, loadedDelegate);
	return true;
}

void UDataManagerSubsystem::SaveCurrentGameplay()
{
	if (gameMode == nullptr) GetGameMode();
	FString saveSlot = gameMode->GetSaveSlotName();

	saveSlot = GetNewName(saveSlot);
	SaveCurrentGameplay(saveSlot);
}

void UDataManagerSubsystem::SaveGameplay(FGameplayDataStruct dataStruct)
{
	if (UGameDataSaveGame* saveGameInstance = 
		Cast<UGameDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UGameDataSaveGame::StaticClass())))
	{
		FAsyncSaveGameToSlotDelegate savedDelegated;
		savedDelegated.BindUObject(this, &UDataManagerSubsystem::SaveGameCallback);
		
		saveGameInstance->gameplayData = dataStruct;
		saveGameInstance->saveSlotName = dataStruct.levelData.gameName;

		UGameplayStatics::AsyncSaveGameToSlot(
			saveGameInstance, saveGameInstance->saveSlotName, saveGameInstance->userIndex, savedDelegated);
	}
}

void UDataManagerSubsystem::OverrideGameplay(FString saveSlot)
{
	SaveCurrentGameplay(saveSlot);
}

void UDataManagerSubsystem::CreateNewGame(FString slotName, FPlayerDataStruct playerstats)
{
	slotName = GetNewName(slotName);

	defaultGame.levelData = FLevelDataStruct();
	defaultGame.levelData.gameName = slotName;
	defaultGame.levelData.playerData = playerstats;
	defaultGame.itemsCollected = 0;
	defaultGame.time = 0;
	defaultGame.timerSpeed = 0.01f;

	selectedSlot.characterID = playerstats.characterID;
	selectedSlot.slotName = slotName;
}

void UDataManagerSubsystem::SetSelectedSlot(FSaveSlotMetaData slot)
{
	defaultGame.levelData = FLevelDataStruct();
	defaultGame.levelData.playerData = FPlayerDataStruct();
	defaultGame.levelData.playerData.characterID = slot.characterID;
	defaultGame.levelData.gameName = slot.slotName;

	selectedSlot = slot;
}


void UDataManagerSubsystem::SaveGameCallback(const FString& slotName, const int32 userIndex, bool bSuccess)
{
	savedEvent.Broadcast(bSuccess);
}

void UDataManagerSubsystem::LoadGameCallback(const FString& slotName, const int32 userIndex, USaveGame* loadedGameData)
{
	UGameDataSaveGame* dataGame = Cast<UGameDataSaveGame>(loadedGameData);

	if (dataGame) loadedEvent.Broadcast(dataGame->gameplayData);
}

void UDataManagerSubsystem::SaveCurrentGameplay(FString saveSlot)
{
	if (UGameDataSaveGame* saveGameInstance = 
		Cast<UGameDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UGameDataSaveGame::StaticClass())))
	{
		FAsyncSaveGameToSlotDelegate savedDelegated;
		savedDelegated.BindUObject(this, &UDataManagerSubsystem::SaveGameCallback);
		
		if (gameMode == nullptr) GetGameMode();
		FGameplayDataStruct gameplayData = gameMode->GetGameplayData();

		saveGameInstance->gameplayData = gameplayData;	
		saveGameInstance->saveSlotName = saveSlot;

		UGameplayStatics::AsyncSaveGameToSlot(
			saveGameInstance, saveGameInstance->saveSlotName, saveGameInstance->userIndex, savedDelegated);
	}
}

FString UDataManagerSubsystem::GetNewName(FString source)
{
	int32 counter = 1;
	FString newName = source;
	while(UGameplayStatics::DoesSaveGameExist(newName, 0))
	{
		newName = FString::Printf(TEXT("%s_%d"),*source,counter);
		counter++;
	}

	return newName;
}

void UDataManagerSubsystem::GetGameMode()
{
	gameMode = CastChecked<APlatformGameMode>(GetWorld()->GetAuthGameMode());
}

