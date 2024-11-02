// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/OrbsEffectManagerSubsystem.h"
#include "Subsystems/DataManagerSubsystem.h"
#include "StackOBotPlatform/Plataformer/PlatformGameMode.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Misc/Paths.h"

void UOrbsEffectManagerSubsystem::CollectOrb(FOrbDataStruct orbData)
{
	if (orbData.effectData.effectType == FOrbEffectType::ORB_POSSITIVE)
		StoreOrb(orbData);
	else
		ApplyEffect(orbData);
}

void UOrbsEffectManagerSubsystem::StoreOrb(FOrbDataStruct orbData)
{
	orbStoredEvent.Broadcast(orbData);
}

void UOrbsEffectManagerSubsystem::ApplyEffect(FOrbDataStruct orbData)
{
	if (orbData.orbID == FOrbID::ORB_Random)
	{
		ApplyEffect(GetRandomOrbData());
		return;
	}

	GetGameMode();
	if (!gameMode) return;

	//we need to keep the id of the orbData to show it when we discard the effect
	FOrbDataStruct beforeOrbEffect = orbData;

	bool getDataSuccess;
	auto effectData = gameMode->GetCurrentOrbEffectData(getDataSuccess);
	if (getDataSuccess)
		beforeOrbEffect.effectData = effectData;

	float effectTime = orbData.effectData.effectTimer;

	if (orbData.effectData.effectTimer != -1)
		CreateStateEffect(orbData, beforeOrbEffect);

	if (gameMode->GetOrbEffectsCounter() > 0)
		AddEffect(orbData);
	else
		gameMode->ApplyOrbEffect(orbData);

	effectAppliedEvent.Broadcast(orbData);
}

void UOrbsEffectManagerSubsystem::CreateStateEffect(FOrbDataStruct orbData, FOrbDataStruct dataBeforeEffect)
{
	if (effectsReferences.Find(orbData.orbID))
	{
		effectsReferences[orbData.orbID].currentTime += orbData.effectData.effectTimer;
		return;
	}

	FStateTransitionOrbEffectStruct managementStruct;
	managementStruct.Construct(orbData,dataBeforeEffect);

	managementStruct.currentTime = managementStruct.effectTime;
    effectsReferences.Add(managementStruct.orbID,managementStruct);
	OrbsApplaying.Add(managementStruct.orbID);
	orbStateEffectEvent.Broadcast(managementStruct.orbID, orbData);
}

void UOrbsEffectManagerSubsystem::DiscardEffect(FOrbDataStruct beforeEffectOrbData)
{
	GetGameMode();
	if (!gameMode) return;

	if (gameMode->GetOrbEffectsCounter() > 1)
		SubstractEffect(beforeEffectOrbData);
	else
	gameMode->DiscardEffect(beforeEffectOrbData);

	discardEvent.Broadcast(beforeEffectOrbData);
}

void UOrbsEffectManagerSubsystem::AddEffect(FOrbDataStruct orbData)
{
	GetGameMode();
	FOrbEffectData orbEffect = orbData.effectData;

	bool getDataSuccess;
	FOrbEffectData currentEffect = gameMode->GetCurrentOrbEffectData(getDataSuccess);
	if(getDataSuccess)
		orbData.effectData = currentEffect + orbEffect;
	
	gameMode->ApplyOrbEffect(orbData);
}

void UOrbsEffectManagerSubsystem::SubstractEffect(FOrbDataStruct orbData)
{
	GetGameMode();
	FOrbEffectData beforeOrbEffectData;

	bool getDataSuccess;
	FOrbEffectData currentEffect = gameMode->GetCurrentOrbEffectData(getDataSuccess);
	if(getDataSuccess)
		orbData.effectData = currentEffect - beforeOrbEffectData;

	gameMode->DiscardEffect(orbData);
}

void UOrbsEffectManagerSubsystem::GetGameMode()
{
	if(!gameMode) gameMode = Cast<APlatformGameMode>(GetWorld()->GetAuthGameMode());
}

FOrbDataStruct UOrbsEffectManagerSubsystem::GetRandomOrbData()
{
	if (gameMode == nullptr)
		GetGameMode();
	FOrbDataStruct* data = gameMode->GetRandomOrb();
	
	if (data->orbID == FOrbID::ORB_Random)
		return GetRandomOrbData();
	else
		return *data;
}

void UOrbsEffectManagerSubsystem::BindDataManagerSubsystem()
{
	if (dataManagerSubsys) return;
	UWorld* world = GetWorld();
	if (!world) return;

	UGameInstance *gameInstance = world->GetGameInstance();
	if (gameInstance)
		dataManagerSubsys = gameInstance->GetSubsystem<UDataManagerSubsystem>();

	if(dataManagerSubsys)
		dataManagerSubsys->OnLoadedEvent().AddUObject(this, &UOrbsEffectManagerSubsystem::OnGameDataLoadedCallback);
}

bool UOrbsEffectManagerSubsystem::IsOrbEffectApplying(FOrbID OrbID)
{
	return OrbsApplaying.Contains(OrbID);
}

void UOrbsEffectManagerSubsystem::OnGameDataLoadedCallback(FGameplayDataStruct data)
{
	effectsReferences = data.effectsRefs;
	GetGameMode();

	for (auto& orbStateEffectPair : effectsReferences)
	{
		auto currentOrb = orbStateEffectPair.Value.currentOrbData;
		orbStateEffectEvent.Broadcast(orbStateEffectPair.Key, currentOrb);
		effectAppliedEvent.Broadcast(currentOrb);

		gameMode->ApplyOrbEffect(currentOrb);
	}
}

void UOrbsEffectManagerSubsystem::Tick(float deltaTime)
{
	int32 values = effectsReferences.Num();
	
	if (bShouldRemoveOrbInNextFrame)
	{
		effectsReferences.Remove(ToRemoveOrbID);
		bShouldRemoveOrbInNextFrame = false;
	}

	for (auto& orbStateEffectPair : effectsReferences)
		OnOrbEffectTick(orbStateEffectPair.Value, deltaTime);
}

void UOrbsEffectManagerSubsystem::OnOrbEffectTick(FStateTransitionOrbEffectStruct& stateOrbEffect, float deltaTime)
{
	if (stateOrbEffect.currentTime > 0) 
	{
		stateOrbEffect.currentTime -= deltaTime;
		orbStateUpdatedEvent.Broadcast(stateOrbEffect.orbID, stateOrbEffect.currentTime);
	}
	else
	{
		if (effectsReferences.Find(stateOrbEffect.orbID))
		{
			stateOrbEffect.currentTime = stateOrbEffect.effectTime;

			if (OrbsApplaying.Contains(stateOrbEffect.orbID))
				OrbsApplaying.Remove(stateOrbEffect.orbID);

			DiscardEffect(stateOrbEffect.beforeEffectData);
			orbStateDiscardEvent.Broadcast(stateOrbEffect.orbID);
			bShouldRemoveOrbInNextFrame = true;
			ToRemoveOrbID = stateOrbEffect.orbID;
		}
	}
}
