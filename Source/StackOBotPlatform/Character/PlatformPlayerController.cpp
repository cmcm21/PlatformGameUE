// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformPlayerController.h"

#include "PlatformerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "PlatformerCameraManager.h"

#include "GameFramework/Character.h" //ACharacter
#include "StackOBotPlatform/Character/PlatformCharacter.h"//APlatformCharacter
#include "PlatformCharacter.h"
#include "Kismet/GameplayStatics.h" //UGameplayStatics
#include "StackOBotPlatform/Plataformer/PlatformGameMode.h" //APlatformGameMode

APlatformPlayerController::APlatformPlayerController()
{

}

void APlatformPlayerController::BeginPlay()
{
	APlatformCharacter* PlatformCharacter = Cast<APlatformCharacter>(GetCharacter());
	if (PlayerCameraManager)
		CameraManager = Cast<APlatformerCameraManager>(PlayerCameraManager);

	if (CameraManager && PlatformCharacter)
	{
		CameraManager->SetCharacter(PlatformCharacter);
		CameraManager->SetFollowCameraBoom(PlatformCharacter->GetFollowBoom());
		CameraManager->SetCharacterFollowCamera(PlatformCharacter->GetFollowCamera());
		CameraManager->SetCharacterDeathCamera(PlatformCharacter->GetDeathCamera());

		CameraManager->RunStartingCamera();
	}
}
