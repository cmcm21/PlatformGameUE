// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerCameraShake.h"


void UPlatformerCameraShake::Shake(APlayerCameraManager *Camera, 
	float Scale, ECameraShakePlaySpace InPlaySpace, FRotator UserPlaySpaceRot, float duration)
{
	StartShake(Camera, Scale, InPlaySpace, UserPlaySpaceRot);
}
