// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformPlayerController.generated.h"


class APlatformerCameraManager;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API APlatformPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APlatformPlayerController();
	
	virtual void BeginPlay() override;
	FORCEINLINE APlatformerCameraManager* GetCameraManager() { return CameraManager; }

protected:
	APlatformerCameraManager* CameraManager;
};
