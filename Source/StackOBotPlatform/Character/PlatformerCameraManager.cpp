 // Fill out your copyright notice in the Description page of Project Settings.  

#include "PlatformerCameraManager.h" //APlatoformerCameraManager
#include "PlatformCharacter.h" //APlatformCharacter
#include "Camera/CameraComponent.h"// UCameraComponent
#include "PlatformerCameraShake.h" //APlatformerCameraShake
#include "CameraStates/CameraStates.h" //CameraStates 
#include "GameFramework/CharacterMovementComponent.h" //UCharacterMovementComponent
#include "PlatformCameraTypes.h" //ICameraStateInterface


void APlatformerCameraManager::UpdateCamera(float DeltaTime)
{
	if (CharacterFollowCamera && Character)
	{
		FTransform FollowCameraTransform = CharacterFollowCamera->GetRelativeTransform();
		SetActorLocation(FollowCameraTransform.GetLocation());
		SetActorRotation(FollowCameraTransform.GetRotation());

		//SetFOV(90);
	}
	
	if (CameraState != nullptr)
		CameraState->OnUpdate(DeltaTime, this);

	if (State == ECameraManagerState::ECMS_STARTING)
	{
		StartingCameraTimer += DeltaTime;
		if (StartingCameraTimer >= CameraStartingSettings.StartingCameraAnimationTimer)
		{
			State = ECameraManagerState::ECMS_IDLE;
			ChangeState(new CameraStateMovement);
		}
	}
	else if(Character && Character->GetCharacterState() == ECharacterState::ECS_IDLE)
	{
		IdleTimer += DeltaTime;
		if (IdleTimer >= 5.0f)
			ChangeState(new CameraStateIdle);
	}

	Super::UpdateCamera(DeltaTime);
}

void APlatformerCameraManager::Shake(float duration)
{
	if (!CameraShake) return;
	StartCameraShake(CameraShake);
}

void APlatformerCameraManager::StopShake()
{
	UCameraShakeBase* ShakeBase = Cast<UCameraShakeBase>(CameraShake);
	if(ShakeBase)
		StopCameraShake(ShakeBase);
}

void APlatformerCameraManager::ChangeState(ICameraStateInterface* NewState)
{
	if (State == ECameraManagerState::ECMS_STARTING || State == ECameraManagerState::ECMS_ENDING) return;
	if (Character && Character->IsUnderOrbEffect())
		return;

	if (CameraState != nullptr)
		CameraState->OnExit(this);

	if (Cast<CameraStateIdle>(NewState) == nullptr)
		IdleTimer = 0;
		
	CameraState = NewState;
	CameraState->OnEnter(this);
}

void APlatformerCameraManager::RunStartingCamera()
{
	if (!bRunStartingCamera) return;

	CameraStartingState = new CameraStateStarting();
	CameraStartingState->SetCameraSettings(CameraStartingSettings);
	StartingCameraTimer = 0;

	ChangeState(CameraStartingState);
	State = ECameraManagerState::ECMS_STARTING;
}

void APlatformerCameraManager::RunEndingCamera()
{
	if (!bRunEndingCamera) return;

	CameraEndingState = new CameraStateEnding();
	CameraEndingState->SetCameraSettings(CameraEndingSettings);

	ChangeState(CameraEndingState);
	State = ECameraManagerState::ECMS_ENDING;
}

void APlatformerCameraManager::OnTimerEndCallback()
{
	UCameraShakeBase *BaseShake = Cast<UCameraShakeBase>(CameraShake);
	StopCameraShake(BaseShake, true);
}
