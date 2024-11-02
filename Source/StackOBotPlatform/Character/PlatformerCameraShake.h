// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "PlatformerCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UPlatformerCameraShake : public UCameraShakeBase
{

	GENERATED_BODY()

public:
	void Shake(APlayerCameraManager *Camera, float Scale, ECameraShakePlaySpace InPlaySpace, FRotator UserPlaySpaceRot, float duration);
};
