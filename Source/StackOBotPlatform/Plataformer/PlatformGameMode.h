// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StackOBotPlatform/GameModeData.h"
#include "StackOBotPlatform/Public/Subsystems/DataManagerSubsystem.h"
#include "PlatformGameMode.generated.h"

//class UItemsPlatformUserWidget;
class UGameoverWidget;
class USaveScore;
class APlatformCharacter;
class ACollectableItem;
class UOrbsEffectManagerSubsystem;
class UInventoryManagerSubsystem;
class APlatformPlayerController;

UENUM(BlueprintType)
enum class EGameModeState : uint8
{
	GS_WIN UMETA(DisplayName="Win"),
	GS_LOSE UMETA(DisplayName="Lose"),
	GS_NOOVER UMETA(DisplayName="No Over"),
	GS_PAUSE UMETA(DisplayName="Pause"),
	GS_SAVE_AREA UMETA(DisplayName="In Save Area"),
	GS_PLAYER_RESTARTING UMETA(DisplayName="Restarting")
};

UCLASS()
class STACKOBOTPLATFORM_API APlatformGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	APlatformGameMode();
	
	/// <summary>
	/// Events
	/// </summary>
	DECLARE_EVENT(APlatformGameMode, FGameoverEvent)
	FGameoverEvent& OnGameoverEvent() { return gameoverEvent; };

	DECLARE_EVENT_OneParam(APlatformGameMode, FTimeUpdatedEvent, float)
	FTimeUpdatedEvent& OnTimeUpdatedEvent() { return timeUpdatedEvent; }

	DECLARE_EVENT_TwoParams(APlatformGameMode, FItemCollectedEvent, int32, int32)
	FItemCollectedEvent& OnItemCollectedEvent() { return itemCollectedEvent; }

	DECLARE_EVENT_TwoParams(APlatformGameMode, FHPUpdatedEvent, int32, int32)
	FHPUpdatedEvent& OnHpUpdatedEvent() { return hpUpdatedEvent; }

	DECLARE_EVENT_ThreeParams(APlatformGameMode, FHPMaxUpdatedEvent, int32, int32, int32)
	FHPMaxUpdatedEvent& OnHpMaxUpdatedEvent() { return hpMaxUpdatedEvent; }

	DECLARE_EVENT_TwoParams(APlatformGameMode, FExpUpdatedEvent, int32, int32)
	FExpUpdatedEvent& OnExpUpdatedEvent() { return expUpdatedEvent; }

	DECLARE_EVENT_OneParam(APlatformGameMode, FLevelUpdatedEvent, int32)
	FLevelUpdatedEvent& OnLevelUpdatedEvent() { return lvlUpdatedEvent; }

	DECLARE_EVENT(AplatformGameMode, FSavePointCheckEvent)
	FSavePointCheckEvent& OnSavePointCheckedEvent() { return savePointCheckEvent; }

	/// <summary>
	/// Blueprint functions
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameModeState GetGameState() { return gameState; };

	UFUNCTION(BlueprintCallable, Category="Gameplay Management")
	FORCEINLINE bool GameWon() { return gameState == EGameModeState::GS_WIN; };

	UFUNCTION()
	bool IsGameOver();

	UFUNCTION(BlueprintCallable)
	void GameoverFromBP(bool won);

	UFUNCTION(BlueprintCallable, Category="Level Data")
	FLevelDataStruct GetCurrentLevelData();

	UFUNCTION(BlueprintCallable,Category="Gameplay Score Data")
	FScoreDataStruct GetScoreData();

	UFUNCTION(BlueprintCallable, Category="Gameplay Data")
	FGameplayDataStruct GetGameplayData();

	/// <summary>
	/// API
	/// </summary>
	void SaveDataBeforePlayerDestroy(FPlayerDataStruct playerData);
	void RefreshPlayerData(FPlayerDataStruct playerData);
	/// <summary>
	///	It is used for keep the counter of the effects.
	/// it calls internally SetEffectData
	/// </summary>
	/// <param name="dataStruct"></param>
	void ApplyOrbEffect(FOrbDataStruct dataStruct);
	/// <summary>
	///	Used for keep the counter of the effects 
	/// it substract 1 to the effects counter and ensure the value doesn't go below 0
	/// </summary>
	/// <param name="orbData"></param>
	void DiscardEffect(FOrbDataStruct orbData);
	void PlayerDestroyed(AController* newPlayer);

	//needed to setup player at beggining and at restarting
	virtual void RestartPlayer(AController* player) override;

	void StoreSavePoint(FVector savePointPosition);
	void ItemCollected(ACollectableItem* item);
	void OnEnterSaveArea();
	void OnExitSaveArea();

	FORCEINLINE FString GetSaveSlotName() { return levelData.gameName; }
	FORCEINLINE void ResendItemsEvent() { itemCollectedEvent.Broadcast(totalItems, itemsCollected); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetTime() { return currentTime;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTime(float Time) { currentTime = Time; }

	FOrbDataStruct* GetRandomOrb();
	/// <summary>
	///	Used to Get the data of the player with the last orb effect applied 
	/// </summary>
	/// <returns></returns>
	FOrbEffectData GetCurrentOrbEffectData(bool &bOutSuccess);
	/// <summary>
	/// Used to get the data of the player before any orb effect was applied
	/// </summary>
	/// <returns></returns>
	FORCEINLINE FOrbEffectData GetBeforeAllOrbEffectData() { return beforeEffectsData; }
	/// <summary>
	/// Get the current orbs effect counter
	/// </summary>
	/// <returns></returns>
	FORCEINLINE int32 GetOrbEffectsCounter(){ return orbEffectsCounter; }

protected:

	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gameplay Configuration")
	int32 totalItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gameplay Configuration")
	int32 itemsCollected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay Configuration")
	int32 timerInSeconds;

	//////////////Game Data Tables 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Leveling Data")
	UDataTable* levelingDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player stats data")
	UDataTable* playerDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Orbs Data Table")
	UDataTable* orbsDataTable;;
	/////////////////

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Data")
	FLevelDataStruct levelData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	AController* respawnPlayer;

	UFUNCTION(BlueprintPure)
	bool IsLoading() { return bIsLoading; }


private:

	/// <summary>
	/// Gameplay data
	/// </summary>
	float currentTime = 0;
	float timerSpeed = 0.01;
	float timerCounter = 0.01;
	int32 playerFalls = -1;
	int32 playerMaxHp;
	int32 lastPlayerLevel = 0;
	FTimerHandle timerHandle;
	EGameModeState gameState = EGameModeState::GS_NOOVER;
	FVector lastSavePoint;
	APlatformCharacter* character;
	int32 currentPlayerHP = 10;

	/// <summary>
	/// Data Management variables
	/// </summary>
	int32 currentLevelingRow = 0;
	bool bIsLoading = false;
	TArray<FLevelingInformationTable *> levelingRows;
	FOrbEffectData beforeEffectsData;
	int32 orbEffectsCounter;

	/// <summary>
	/// Events variables
	/// </summary>
	FGameoverEvent gameoverEvent;
	FTimeUpdatedEvent timeUpdatedEvent;
	FItemCollectedEvent itemCollectedEvent;
	FHPUpdatedEvent hpUpdatedEvent;
	FHPMaxUpdatedEvent hpMaxUpdatedEvent;
	FExpUpdatedEvent expUpdatedEvent;
	FLevelUpdatedEvent lvlUpdatedEvent;
	FSavePointCheckEvent savePointCheckEvent;
	UOrbsEffectManagerSubsystem* orbsEffectManagerSubsys;
	UInventoryManagerSubsystem* inventoryManagerSubsys;

	/// <summary>
	/// Events callbacks
	/// </summary>
	void OnTimerCallback();
	void OnPlayerLevelUpdated(int32 newLvl);
	void OnPlayerExpUpdated(int32 nextLevelExp,int32 exp);
	void OnPlayerLevelUp();
	void OnPlayerHPUpdated();
	void OnDataManagerSubsystemLoadData(FGameplayDataStruct data);

	/// <summary>
	/// Internal class functions
	/// </summary>
	void Gameover();
	void SetupCollectables();
	bool IsOrbValid(FString &orbName);
	void SetupPlayer();
	void SetupLevelingData();
	void HealthPlayer(int32 value);
	void InitValues();
	void LoadMapData();
	void LoadDataToPlayer();
	void LoadPlayer();
	bool BindDataManagerSubsystem();
	void ResetGameTimer();
	void GetOrbsEffectManagerSubsys();
	void GetInventoryManagerSubsys();

	/// <summary>
	///	Used to set effect data to player, it doesn't disturb effects counter,
	/// it can be called for substract or add effects  
	/// </summary>
	/// <param name="effectData"></param>
	void SetOrbEffectData(FOrbEffectData effectData);

	APlatformCharacter* GetPlatformCharacter();
	UDataManagerSubsystem* GetDataManagerSubSystem();
};
