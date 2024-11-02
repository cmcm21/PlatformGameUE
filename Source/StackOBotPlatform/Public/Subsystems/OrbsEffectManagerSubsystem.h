// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StackOBotPlatform/GameModeData.h"
#include "OrbsEffectManagerSubsystem.generated.h"


struct FOrbDataStruct;
struct FOrbEffectData;
class APlatformCharacter;
class APlatformGameMode;
class UDataManagerSubsystem;
/**
 * FStateOrbEffectStruct
 * *
 * This struct is used to manage the states transition  between 
 * the last orb effect and the next orb effect when the effects have
 * a timer
 */

UCLASS()
class STACKOBOTPLATFORM_API UOrbsEffectManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	const FString orbsDataTablePath = "OrbDataTable.uasset";
	int32 stateOrbEffectID = 0;

	DECLARE_EVENT_OneParam(UOrbsEffectManagerSubsystem, FEffectAppliedEvent,FOrbDataStruct)
	FEffectAppliedEvent& OnOrbEffectApplied() { return effectAppliedEvent; }

	DECLARE_EVENT_OneParam(UOrbsEffectManagerSubsystem, FOrbStoredEvent,FOrbDataStruct)
	FOrbStoredEvent& OnOrbStoredEvent() { return orbStoredEvent; }

	DECLARE_EVENT_OneParam(UOrbsEffectManagerSubsystem, FOrbDiscardEffectEvent, FOrbDataStruct)
	FOrbDiscardEffectEvent& OnOrbEffectDiscardEvent() { return discardEvent; }

	DECLARE_EVENT_TwoParams(UOrbsEffectManagerSubsystem, FOrbStateEffectApplied, FOrbID, FOrbDataStruct)
	FOrbStateEffectApplied& OnStateOrbEffectApplied() { return orbStateEffectEvent; }

	DECLARE_EVENT_TwoParams(UOrbsEffectManagerSubsystem, FOrbStateTimeUpdated, FOrbID, float)
	FOrbStateTimeUpdated& OnOrbStateTimeUpdated() { return orbStateUpdatedEvent; }

	DECLARE_EVENT_OneParam(UOrbsEffectManagerSubsystem, FOrbStateDiscard, FOrbID)
	FOrbStateDiscard& OnOrbStateDiscard() { return orbStateDiscardEvent; }

	virtual void Tick(float deltaTime) override;
	virtual TStatId GetStatId() const { return Super::GetStatID(); };

	void CollectOrb(FOrbDataStruct orbData);
	void StoreOrb(FOrbDataStruct orbData);
	void ApplyEffect(FOrbDataStruct orbData);
	void DiscardEffect(FOrbDataStruct discardEffect);
	TMap<FOrbID, FStateTransitionOrbEffectStruct> &GetEffectReferences() { return effectsReferences; }
	void LoadOrbsReferences(TMap<FOrbID, FStateTransitionOrbEffectStruct> orbsRefs) { effectsReferences = orbsRefs; }
	void BindDataManagerSubsystem();
	bool IsOrbEffectApplying(FOrbID OrbID);

private:
	//events
	FEffectAppliedEvent effectAppliedEvent;
	FOrbStoredEvent orbStoredEvent;
	FOrbDiscardEffectEvent discardEvent;
	FOrbStateEffectApplied orbStateEffectEvent;
	FOrbStateTimeUpdated orbStateUpdatedEvent;
	FOrbStateDiscard orbStateDiscardEvent;
	

	//variables
	UDataTable* orbsDataTable;
	TMap<FOrbID, FStateTransitionOrbEffectStruct> effectsReferences;
	TSet<FOrbID> OrbsApplaying;
	FOrbID ToRemoveOrbID;
	bool bShouldRemoveOrbInNextFrame;

	const float timeSpeed = 0.01f;
	APlatformGameMode *gameMode;
	UDataManagerSubsystem* dataManagerSubsys;

	void AddEffect(FOrbDataStruct orbData);
	void SubstractEffect(FOrbDataStruct orbData);
	void GetGameMode();

	/// <summary>
	/// We use this methond to create a state effect struct and broadcast the 
	/// a new effect is beging appied event
	/// </summary>
	/// <param name="orbData"></param>
	/// <param name="dataBeforeEffect"></param>
	void CreateStateEffect(FOrbDataStruct orbData, FOrbDataStruct dataBeforeEffect);
	void OnOrbEffectTick(FStateTransitionOrbEffectStruct &effect, float deltaTime);
	FOrbDataStruct GetRandomOrbData();
	void OnGameDataLoadedCallback(FGameplayDataStruct data);
};

