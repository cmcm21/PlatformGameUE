// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StackOBotPlatform/GameModeData.h"
#include "Engine/DataTable.h"
#include "CollectableItem.generated.h"

class UStaticMeshComponent;
class UPrimitiveComponent;
class APlatformGameMode;
class APlatformCharacter;
class UOrbsEffectManagerSubsystem;
class UInventoryManagerSubsystem;

UCLASS()
class STACKOBOTPLATFORM_API ACollectableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableItem();
	virtual void Tick(float DeltaTime) override;

	bool IsEnabled(){ return enabled; };

	void SetVisibility(bool value);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* mesh;

	UFUNCTION(BlueprintNativeEvent)
	void Collected();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool enabled = true;

	UPROPERTY(EditAnywhere, Category = "Orb Data")
	FDataTableRowHandle orbDataRowHandle;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& propertyEvent) override;
#endif

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float SinTransform();
	APlatformGameMode* gameMode;
	APlatformCharacter* character;
	FOrbDataStruct* orbData;
	UOrbsEffectManagerSubsystem *orbsSubsys;
	UInventoryManagerSubsystem *inventoryManagerSubsys;

	float runningTime;
	float amplitude = 0.25f;
	float timeScaled = 1.5f;

	void GetOrbSubsystem();
	void GetInventorySubsystem();
};
