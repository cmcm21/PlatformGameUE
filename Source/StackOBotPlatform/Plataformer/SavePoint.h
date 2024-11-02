// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SavePoint.generated.h"

class UStaticMeshComponent;
class APlatformGameMode;
class APlatformHUD;

UCLASS()
class STACKOBOTPLATFORM_API ASavePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	virtual void BeginPlay() override;

	ASavePoint();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	UStaticMeshComponent* mesh;

	UFUNCTION(BlueprintCallable)
	void OnPlayerHit();

	UFUNCTION(BlueprintCallable)
	void OnPlayerEnter();

	UFUNCTION(BlueprintCallable)
	void OnPlayerExit();

private:
	APlatformGameMode* gameMode;
	void GetGameMode();

	APlatformHUD* HUD;
	void GetHUD();
};
