// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "GameModeData.generated.h"

class UMaterial;
class UTexture2D;

UENUM(BlueprintType)
enum class FOrbID : uint8
{
	ORB_Max_Health UMETA(DisplayName = "Increase Max Health"),
	ORB_Jump UMETA(DisplayName = "Jump"),
	ORB_Normal UMETA(DisplayName = "Normal"),
	ORB_SlowDown UMETA(DisplayName = "Slow Down"),
	ORB_SpeedUp UMETA(DisplayName =  "Speed Up"),
	ORB_NoJump UMETA(DisplayName =  "No Jump"),
	ORB_Random UMETA(DisplayName =  "Random")
};

UENUM(BlueprintType)
enum class FOrbEffectType : uint8
{
	ORB_POSSITIVE UMETA(DisplayName="Possitive Effect"),
	ORB_NEGATIVE UMETA(DisplayName="Negative Effect")
};

UENUM(BlueprintType)
enum class HUDWidgetID: uint8
{
	HUD_INVENTOR_W,
	HUD_TIME_W,
	HUD_ORB_COLLECTION_W,
	HUD_SAVE_W,
	HUD_ORBS_INFO_W,
	HUD_PLAYER_STATS_W,
	HUD_SAVE_POINT_W,
	HUD_ORB_EFFECT_STATE_W_SINGLE,
	HUD_ORB_EFFECT_STATE_W_LIST,
	HUD_DIED_W
};

UENUM(BlueprintType)
enum class HUDMenuID: uint8
{
	HUD_PAUSE_M,
	HUD_SAVE_M,
	GAME_OVER_M,
	SCORE_BOARD_M
};

UENUM(BlueprintType)
enum class GameoverState : uint8
{
	GAME_OVER,
	NEW_SCORE,
	NO_NEW_SCORE,
};


USTRUCT(BlueprintType)
struct FGenericLevelingInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level data")
	int32 level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level data")
	int32 exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level data")
	int32 numOfJumps;
};

USTRUCT(BlueprintType)
struct FLevelingInformationTable : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Data")
	int32 level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Data")
	int32 exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Data")
	int32 jumps;
};

USTRUCT(BlueprintType, Category = "Gameplay Score")
struct FScoreDataStruct: public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay Score")
	FString gameplayName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay Score")
	FString character = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay Score")
	float time = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay Score")
	int32 falls = 0;
};

USTRUCT(BlueprintType, Category = "Player Data")
struct FPlayerDataStruct: public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	int32 characterID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	FString characterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	int32 maxHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	int32 currentHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	float jumpHigh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	float maxSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	bool canJump = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	int32 level;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Data")
	int32 exp;
};


USTRUCT(BlueprintType, Category="Level Data")
struct FLevelDataStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Data")
	FString gameName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Data")
	FPlayerDataStruct playerData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Data")
	TArray<FString> itemsCollected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Data")
	int32 levelingRow;
};

USTRUCT(BlueprintType, Category = "Orb effect Data")
struct FOrbEffectData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 hpIncrease = 1;

	UPROPERTY(EditAnywhere)
	float effectTimer = -1;

	UPROPERTY(EditAnywhere)
	FOrbEffectType effectType;

	UPROPERTY(EditAnywhere)
	float timeSpeed = -1;

	UPROPERTY(EditAnywhere)
	float healthPercentage = -1;

	UPROPERTY(EditAnywhere)
	float jumpForcePercentage = -1;

	UPROPERTY(EditAnywhere)
	float jumpHeight = -1;

	FOrbEffectData &operator+(const FOrbEffectData &other)
	{
		if(other.timeSpeed != -1)
			timeSpeed = other.timeSpeed;
		if(other.hpIncrease != -1)
			hpIncrease = other.hpIncrease;
		if (other.jumpForcePercentage != -1)
			jumpForcePercentage = other.jumpForcePercentage;
		if (other.jumpHeight != -1)
			jumpHeight = other.jumpHeight;
		if (other.healthPercentage != -1)
			healthPercentage = other.healthPercentage;

		return *this;
	}

	FOrbEffectData &operator-(const FOrbEffectData &otherEffect)
	{
		if(otherEffect.timeSpeed <= -1)
			timeSpeed = otherEffect.timeSpeed;
		if(otherEffect.hpIncrease <= -1)
			hpIncrease = otherEffect.hpIncrease;
		if (otherEffect.jumpHeight <= -1)
			jumpHeight = otherEffect.jumpHeight;
		if (otherEffect.jumpForcePercentage <= -1)
			jumpForcePercentage = otherEffect.jumpForcePercentage;
		if (otherEffect.healthPercentage <= -1)
			healthPercentage = otherEffect.healthPercentage;

		return *this;
	}
};

USTRUCT(BlueprintType, Category="Orbs Data")
struct FOrbDataStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOrbID orbID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance *orbMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText shortDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOrbEffectData effectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D *orbTexture;
};

USTRUCT(BlueprintType, Category="Orb state transition Data")
struct FStateTransitionOrbEffectStruct
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FOrbID orbID;
	UPROPERTY()
	float effectTime;
	UPROPERTY()
	float currentTime;
	UPROPERTY()
	FOrbDataStruct currentOrbData;
	UPROPERTY()
	FOrbDataStruct beforeEffectData;

	void Construct(FOrbDataStruct bCurrentData, FOrbDataStruct bBeforeEffectData)
	{
		orbID = bCurrentData.orbID;
		effectTime = bCurrentData.effectData.effectTimer;
		currentTime = effectTime;
		currentOrbData = bCurrentData;
		beforeEffectData = bBeforeEffectData;
	}
};


USTRUCT(BlueprintType, Category="Inventory data")
struct FInventoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY() 
	TMap<FOrbID, FOrbDataStruct> orbs;

	UPROPERTY() 
	TMap<FOrbID, int32> counter;

	void AddOrb(FOrbDataStruct newOrb, int32 amountOfOrbs)
	{
		if (orbs.Find(newOrb.orbID))
			counter[newOrb.orbID] += amountOfOrbs;
		else
		{
			orbs.Add(newOrb.orbID,newOrb);
			counter.Add(newOrb.orbID, amountOfOrbs);
		}
	}

	void AddOrb(FOrbDataStruct newOrb)
	{
		if (orbs.Find(newOrb.orbID))
			counter[newOrb.orbID]++;
		else
		{
			orbs.Add(newOrb.orbID, newOrb);
			counter.Add(newOrb.orbID, 1);
		}
	}

	void RemoveOrb(FOrbID orbID)
	{
		if (orbs.Find(orbID))
		{
			counter[orbID]--;
			if (counter[orbID] <= 0)
			{
				counter.Remove(orbID);
				orbs.Remove(orbID);
			}
		}
	}

	void RemoveOrb(FOrbID orbID, int32 amountOfOrbs)
	{
		if (orbs.Find(orbID))
		{
			counter[orbID] -= amountOfOrbs;
			if (counter[orbID] <= 0)
			{
				counter.Remove(orbID);
				orbs.Remove(orbID);
			}
		}
	}

	void RemoveAllOrbs(FOrbID orbID)
	{
		if (orbs.Find(orbID))
		{
			orbs.Remove(orbID);
			counter.Remove(orbID);
		}
	}
};


USTRUCT(BlueprintType, Category="Game Data")
struct FGameplayDataStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	FLevelDataStruct levelData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	FVector lastSavePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	float time;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	float timerSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	int itemsCollected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	int totalItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	TMap<FOrbID, FStateTransitionOrbEffectStruct> effectsRefs;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	FInventoryStruct inventory;
};


USTRUCT(BlueprintType, Category="Slots Data")
struct FSaveSlotMetaData : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	int32 characterID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	FString slotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	float time;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	int32 itemsCollected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Basic)
	int32 totalItems;
};


UCLASS()
class STACKOBOTPLATFORM_API UGameData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
