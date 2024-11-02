#include "PlatformGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "StackOBotPlatform/Plataformer/CollectableItem.h"
#include "EngineUtils.h"
#include "GameoverWidget.h"
#include "StackOBotPlatform/Character/PlatformCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "SaveScore.h"
#include "Math/UnrealMathUtility.h"
#include "StackOBotPlatform/GameDefinitions.h"
#include "Subsystems/OrbsEffectManagerSubsystem.h"
#include "Subsystems/InventoryManagerSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StackOBotPlatform/Public/HUD//PlatformHUD.h"

#include "StackOBotPlatform/Character/PlatformPlayerController.h" //APlatformPlayerController
#include "StackOBotPlatform/Character/PlatformerCameraManager.h" //APlatformCameraManager
#include "StackOBotPlatform/Public/GI_Platformer.h"//UGI_Platformer

APlatformGameMode::APlatformGameMode()
{
	timerInSeconds = 50.0f;
}

void APlatformGameMode::BeginPlay()
{
	Super::BeginPlay();
	FMath::RandInit(FApp::GetCurrentTime());
	InitValues();

	if(!BindDataManagerSubsystem())
	{
		LoadMapData();
		LoadPlayer();
	}
}

void APlatformGameMode::RestartPlayer(AController *player)
{
	if (player == nullptr) return;
	gameState = EGameModeState::GS_NOOVER;

	Super::RestartPlayer(player);
	ACharacter *respawnCharacter = player->GetCharacter();

	if(respawnCharacter)
		character = CastChecked<APlatformCharacter>(respawnCharacter);

	SetupPlayer();

	if (character && lastSavePoint != FVector::ZeroVector)
		character->SetActorLocation(lastSavePoint, false, nullptr, ETeleportType::TeleportPhysics);

	bool getDataSuccess;
	FOrbEffectData orbEffectsBeforePlayerRefresh = GetCurrentOrbEffectData(getDataSuccess);

	if(getDataSuccess) SetOrbEffectData(orbEffectsBeforePlayerRefresh);

	playerFalls++;
	currentPlayerHP -= 2;
	OnPlayerHPUpdated();
}

void APlatformGameMode::InitValues()
{
	UWorld* World = GetWorld();
	UGI_Platformer* GI = World ? Cast<UGI_Platformer>(UGameplayStatics::GetGameInstance(World)) : nullptr;
	currentTime = GI && GI->GetLastLevelTime() != -1 ? GI->GetLastLevelTime() : 0;

	itemsCollected = 0;
	timerSpeed = GAME_TIME_SPEED;
	lastSavePoint = FVector::ZeroVector;

	bool getDataSuccess;
	auto effectData = GetCurrentOrbEffectData(getDataSuccess);
	if (getDataSuccess)
		beforeEffectsData = effectData;
}

void APlatformGameMode::LoadMapData()
{
	SetupCollectables();
	ResetGameTimer();
	SetupLevelingData(); 
}

void APlatformGameMode::LoadPlayer()
{
	SetupPlayer();

	if(character)
		character->SetPlayerData(levelData.playerData);

	LoadDataToPlayer();
}

void APlatformGameMode::ResetGameTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &APlatformGameMode::OnTimerCallback, timerSpeed, true);
}

void APlatformGameMode::GetOrbsEffectManagerSubsys()
{
	if (orbsEffectManagerSubsys) return;
	
	UWorld* world = GetWorld();

	if (world) orbsEffectManagerSubsys = world->GetSubsystem<UOrbsEffectManagerSubsystem>();
}

void APlatformGameMode::GetInventoryManagerSubsys()
{
	if (inventoryManagerSubsys) return;
	UWorld* world = GetWorld();

	if (world) inventoryManagerSubsys = world->GetSubsystem<UInventoryManagerSubsystem>();

	if (character == nullptr || levelingDataTable == nullptr) return;
}

void APlatformGameMode::LoadDataToPlayer()
{
	FString rowName;
	rowName.AppendInt(levelData.playerData.level);

	FLevelingInformationTable* levelingData = 
		levelingDataTable->FindRow<FLevelingInformationTable>(
			FName(*rowName), FString::Printf(TEXT("Generic Context")));

	if (character == nullptr) character = GetPlatformCharacter();
	if (levelingData != nullptr) character->SetCurrentDataTableRow(levelingData);

	character->SetPlayerData(levelData.playerData);
	currentLevelingRow = levelData.levelingRow;

	currentPlayerHP = levelData.playerData.currentHP;
	playerMaxHp = levelData.playerData.maxHP;
	OnPlayerHPUpdated();
	SetupPlayer();
}

bool APlatformGameMode::BindDataManagerSubsystem()
{
	UDataManagerSubsystem* system = GetDataManagerSubSystem();
	GetOrbsEffectManagerSubsys();

	if (orbsEffectManagerSubsys)
		orbsEffectManagerSubsys->BindDataManagerSubsystem();

	if (!system) 
		return false;

	if (system->OnLoadedEvent().IsBoundToObject(this))
		system->OnLoadedEvent().Clear();

	system->OnLoadedEvent().AddUObject(this, &APlatformGameMode::OnDataManagerSubsystemLoadData);
	bIsLoading = system->AsyncLoadGameplayData(system->selectedSlot.slotName);
	return bIsLoading;
}

void APlatformGameMode::OnDataManagerSubsystemLoadData(FGameplayDataStruct data)
{
	bIsLoading = false;
	currentTime = data.time;
	timerSpeed = data.timerSpeed;
	lastSavePoint = data.lastSavePoint;

	itemsCollected = data.itemsCollected;
	totalItems = data.totalItems;
	itemCollectedEvent.Broadcast(totalItems, itemsCollected);

	levelData = data.levelData;
	
	LoadMapData();
	LoadPlayer();
}


void APlatformGameMode::SetupCollectables()
{
	TArray<AActor*> items;
	UWorld* world = GetWorld();
	UGameplayStatics::GetAllActorsOfClass(world, ACollectableItem::StaticClass(), items);
	for(AActor *findCollectable: items)
	{
		ACollectableItem* collectable = Cast<ACollectableItem>(findCollectable);
		if (collectable == nullptr)
			continue;

		FString collectableName = collectable->GetName();
		//we keep the track of the total items in the level. 
		if(collectable->IsEnabled())
			totalItems++;

		if (!IsOrbValid(collectableName))
		{
			collectable->SetVisibility(false);
			collectable->Destroy();
		}
	}

	itemCollectedEvent.Broadcast(totalItems, itemsCollected);
}

bool APlatformGameMode::IsOrbValid(FString &collectableName)
{
	for (FString storedCollectable : levelData.itemsCollected)
		if (storedCollectable == collectableName)
			return false;

	return true;
}

void APlatformGameMode::OnTimerCallback()
{
	if (IsGameOver())
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		return;
	}

	currentTime += timerCounter;
	timeUpdatedEvent.Broadcast(currentTime);
}

void APlatformGameMode::Gameover()
{
	gameoverEvent.Broadcast();
}

void APlatformGameMode::SetupPlayer()
{
	if(!character)
		character = GetPlatformCharacter();

	if (character)
	{
		if (!character->OnExpChanged().IsBoundToObject(this))
			character->OnExpChanged().AddUObject(this, &APlatformGameMode::OnPlayerExpUpdated);

		if (!character->OnLevelChanged().IsBoundToObject(this))
			character->OnLevelChanged().AddUObject(this, &APlatformGameMode::OnPlayerLevelUpdated);

		if (!character->OnLevelUp().IsBoundToObject(this))
			character->OnLevelUp().AddUObject(this, &APlatformGameMode::OnPlayerLevelUp);

		if (levelingRows.Num() > currentLevelingRow)
			character->SetCurrentDataTableRow(levelingRows[currentLevelingRow]);

		//we call set player data not just to update data, but to update UI with all the events already binded as well
		character->SetPlayerData(levelData.playerData);
	}
}

APlatformCharacter* APlatformGameMode::GetPlatformCharacter()
{
	return Cast<APlatformCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

UDataManagerSubsystem* APlatformGameMode::GetDataManagerSubSystem()
{
	return UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDataManagerSubsystem>();
}

bool APlatformGameMode::IsGameOver()
{
	return gameState == EGameModeState::GS_WIN || gameState == EGameModeState::GS_LOSE;
}

void APlatformGameMode::GameoverFromBP(bool won)
{
	gameState = won ? EGameModeState::GS_WIN : EGameModeState::GS_LOSE;
	Gameover();
}

FLevelDataStruct APlatformGameMode::GetCurrentLevelData()
{
	if (character == nullptr)
		character = GetPlatformCharacter();

	levelData.playerData = character->GetPlayerData();
	levelData.playerData.currentHP = currentPlayerHP;
	levelData.playerData.maxHP = playerMaxHp;

	levelData.levelingRow = currentLevelingRow;
	return levelData;
}

FScoreDataStruct APlatformGameMode::GetScoreData()
{
	FScoreDataStruct score;

	score.character = levelData.playerData.characterName;
	score.falls = playerFalls;
	score.time = currentTime;
	
	score.gameplayName = FString::Printf(TEXT("%s-%.4f"), *score.character, score.time);

	return score;
}

void APlatformGameMode::ItemCollected(ACollectableItem *item)
{
	itemsCollected++;
	FString itemName = item->GetName();
	levelData.itemsCollected.Add(item->GetName());
	
	itemCollectedEvent.Broadcast(totalItems, itemsCollected);
}

FGameplayDataStruct APlatformGameMode::GetGameplayData()
{
	FGameplayDataStruct data;
	data.levelData = GetCurrentLevelData();
	data.time = currentTime;
	data.timerSpeed = timerSpeed;
	data.lastSavePoint = lastSavePoint;
	data.itemsCollected = itemsCollected;
	data.totalItems = totalItems;

	GetOrbsEffectManagerSubsys();
	data.effectsRefs = orbsEffectManagerSubsys->GetEffectReferences();
	GetInventoryManagerSubsys();
	data.inventory = inventoryManagerSubsys->GetInventory();

	return data;
}

void APlatformGameMode::PlayerDestroyed(AController* newPlayer)
{
	if (IsGameOver()) return;	
	FLatentActionInfo delayActionInfo;

	APlatformHUD *HUD = Cast<APlatformHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	gameState = EGameModeState::GS_PLAYER_RESTARTING;
	RestartPlayer(newPlayer);

}

void APlatformGameMode::StoreSavePoint(FVector savePointPosition)
{
	if (lastSavePoint != savePointPosition && !lastSavePoint.IsZero())
		savePointCheckEvent.Broadcast();

	lastSavePoint = savePointPosition;
}

void APlatformGameMode::OnPlayerLevelUpdated(int32 levelUpdated)
{
	lvlUpdatedEvent.Broadcast(levelUpdated);
	lastPlayerLevel = levelUpdated;
}

void APlatformGameMode::OnPlayerExpUpdated(int32 nextLevelExp, int32 currentExp)
{
	 expUpdatedEvent.Broadcast(nextLevelExp, currentExp);
}

void APlatformGameMode::OnPlayerLevelUp()
{
	if (!character)
		GetPlatformCharacter();
	if (!character) return;

	currentLevelingRow++;
	currentLevelingRow = FMath::Clamp(currentLevelingRow, 0, levelingRows.Num() - 1);
	character->SetCurrentDataTableRow(levelingRows[currentLevelingRow]);
	OnPlayerLevelUpdated(levelingRows[currentLevelingRow]->level);
	OnPlayerExpUpdated(levelingRows[currentLevelingRow]->exp,0);
}

void APlatformGameMode::OnPlayerHPUpdated()
{
	hpUpdatedEvent.Broadcast(playerMaxHp, currentPlayerHP);
	if (currentPlayerHP <= 0)
	{
		gameState = EGameModeState::GS_LOSE;
		Gameover();
	}
}

void APlatformGameMode::SetupLevelingData()
{
	if (levelingDataTable)
	{
		levelingDataTable->GetAllRows("Leveling Data", levelingRows);
		currentLevelingRow = 0;
	}
}

void APlatformGameMode::SaveDataBeforePlayerDestroy(FPlayerDataStruct playerData)
{
	levelData.playerData = playerData;
}

void APlatformGameMode::RefreshPlayerData(FPlayerDataStruct playerData)
{
	if (character == nullptr)
		character->SetPlayerData(playerData);

	currentPlayerHP = playerData.currentHP;
	playerMaxHp = playerData.maxHP;
	levelData.playerData = playerData;

	OnPlayerHPUpdated();
}

void APlatformGameMode::HealthPlayer(int32 value)
{
	currentPlayerHP += value;
	currentPlayerHP = FMath::Clamp(currentPlayerHP, 0, playerMaxHp);

	OnPlayerHPUpdated();
}

void APlatformGameMode::ApplyOrbEffect(FOrbDataStruct OrbData)
{
	SetOrbEffectData(OrbData.effectData);

	if (OrbData.effectData.effectType == FOrbEffectType::ORB_POSSITIVE)
	{
		bool success;
		auto temp = GetCurrentOrbEffectData(success);
		if (success) beforeEffectsData = temp;
	}

	if (character == nullptr)
		character = GetPlatformCharacter();
	character->ApplyOrbEffect(OrbData);

	orbEffectsCounter++;
}

void APlatformGameMode::DiscardEffect(FOrbDataStruct OrbData)
{
	orbEffectsCounter--;
	orbEffectsCounter = FMath::Clamp(orbEffectsCounter, 0, orbEffectsCounter);

	if (orbEffectsCounter == 0)
		SetOrbEffectData(beforeEffectsData);
	else
		SetOrbEffectData(OrbData.effectData);

	if (character == nullptr)
		character = GetPlatformCharacter();

	character->DiscardOrbEffect(OrbData);
}

void APlatformGameMode::SetOrbEffectData(FOrbEffectData effectData)
{
	if (character == nullptr)
		character = GetPlatformCharacter();

	FPlayerDataStruct playerRawData = character->GetPlayerData();
	if (effectData.healthPercentage != -1)
	{
		int32 oldHP = currentPlayerHP;
		playerRawData.maxHP += (playerRawData.maxHP * ((float)effectData.healthPercentage / 100.0f));
		playerMaxHp = playerRawData.maxHP;
		currentPlayerHP = playerMaxHp;
		hpMaxUpdatedEvent.Broadcast(playerMaxHp, oldHP, currentPlayerHP);
	}
	if (effectData.hpIncrease != -1) 
		HealthPlayer(effectData.hpIncrease);
	if (effectData.jumpForcePercentage != -1)
		playerRawData.jumpHigh = beforeEffectsData.jumpHeight + (beforeEffectsData.jumpHeight * ((float)effectData.jumpForcePercentage / 100.0f));
	if (effectData.jumpHeight != -1)
		playerRawData.jumpHigh = effectData.jumpHeight;
	if (effectData.timeSpeed != -1)
	{
		timerSpeed = effectData.timeSpeed;
		ResetGameTimer();
	}

	if(orbsEffectManagerSubsys)
		playerRawData.canJump = !orbsEffectManagerSubsys->IsOrbEffectApplying(FOrbID::ORB_NoJump);

	character->SetPlayerData(playerRawData);
}

void APlatformGameMode::OnEnterSaveArea()
{
	gameState = EGameModeState::GS_SAVE_AREA;
}

void APlatformGameMode::OnExitSaveArea()
{
	gameState = EGameModeState::GS_NOOVER;
}

FOrbEffectData APlatformGameMode::GetCurrentOrbEffectData(bool &bOutSuccess)
{
	FOrbEffectData data;
	bOutSuccess = true;

	if (!character)
		GetPlatformCharacter();
	if (character)
	{
		data.effectTimer = -1;
		data.healthPercentage = -1;
		data.hpIncrease = -1;
		data.jumpForcePercentage = -1;
		data.timeSpeed = timerSpeed;
		data.jumpHeight = character->GetPlayerData().jumpHigh;
	}

	bOutSuccess = false;
	return data;
}

FOrbDataStruct* APlatformGameMode::GetRandomOrb()
{
	if (orbsDataTable == nullptr) return nullptr;

	TArray<FOrbDataStruct *> rows;
	orbsDataTable->GetAllRows(TEXT("Orbs context"), rows);

	if (rows.IsEmpty()) return nullptr;

	int32 maxNum = rows.Num() - 1;
	int32 randomIndex = FMath::RandRange(0,maxNum);
	FOrbDataStruct* data = rows[randomIndex];
	return data;
}
