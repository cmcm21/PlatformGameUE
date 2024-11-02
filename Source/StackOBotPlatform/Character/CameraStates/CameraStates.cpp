// Fill out your copyright notice in the Description page of Project Settings.

#include "StackOBotPlatform/Character/CameraStates/CameraStates.h"

#include "StackOBotPlatform/Character/PlatformerCameraManager.h" //APlatformerCameraManager
#include "Camera/CameraComponent.h"//UCameraComponent
#include "StackOBotPlatform/Character/PlatformCharacter.h" //APlatformCharacter
#include "GameFramework/SpringArmComponent.h"//USpringArmComponent
#include "Kismet/KismetMathLibrary.h" //UKismetMathLibrary
#include "Kismet/kismetSystemLibrary.h" //EDrawDebugTrace
#include "GameFramework/CharacterMovementComponent.h" //UCharacterMovementComponent
#include "Math/UnrealMathUtility.h" //Clamp


/////////////////////////////CAMERA MOVEMENT//////////////////////////

void CameraStateMovement::OnEnter(APlatformerCameraManager* CameraManager)
{
	PlatformCharacter = CameraManager->GetPlatformCharacter();
	UCameraComponent* FollowCamera = CameraManager->GetFollowCamera();
	if (IsValid(FollowCamera))
		FollowCamera->SetActive(true);

	UCameraComponent* DeathCamera = CameraManager->GetDeathCamera();
	if(IsValid(DeathCamera))
		DeathCamera->SetActive(false);

	UCameraComponent* WinCamera = CameraManager->GetWinCamera();
	if(IsValid(WinCamera))
		WinCamera->SetActive(false);
	
	USpringArmComponent* FollowBoom = CameraManager->GetFollowBoom();
	if (IsValid(FollowBoom))
	{
		FollowBoom->ProbeSize = 0;
		FollowBoom->bDoCollisionTest = false;
	}
}

void CameraStateMovement::OnExit(APlatformerCameraManager* CameraManager)
{

}

void CameraStateMovement::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{
	USpringArmComponent* BoomCamera = CameraManager->GetFollowBoom();

	if (BoomCamera == nullptr || PlatformCharacter == nullptr) return;
	if (PlatformCharacter->GetCharacterState() != ECharacterState::ECS_IDLE) return;

	UCharacterMovementComponent* MovementComponent = PlatformCharacter->GetCharacterMovement();

	if (MovementComponent)
	{
		FVector start = PlatformCharacter->GetActorLocation();
		FVector end = start + FVector(0, 0, 250.0f);
		TArray<AActor*> ActorsToIgnore;
		FHitResult hitResult;

		bool hit = UKismetSystemLibrary::LineTraceSingle(
			PlatformCharacter->GetWorld(),
			start,
			end,
			UEngineTypes::ConvertToTraceType(ECC_Camera),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			hitResult,
			true
		);

		float InterPolationValue = 
			UKismetMathLibrary::FInterpTo(BoomCamera->TargetArmLength, CameraMoveInAir(), DeltaTime, 2.0f);

		BoomCamera->TargetArmLength = InterPolationValue;
	}
}


float CameraStateMovement::CameraMoveInAir()
{
	UCharacterMovementComponent* character = PlatformCharacter->GetCharacterMovement();
	return UKismetMathLibrary::SelectFloat(600, 250, character != nullptr && character->IsFalling());
}

////////////////////////////////////////////////////////////////////

///////////////////////////CAMERA SLOW DOWN/////////////////////////

void CameraStateSlowDown::OnEnter(APlatformerCameraManager* CameraManager)
{

	USpringArmComponent* FollowBoom = CameraManager->GetFollowBoom();

	if (FollowBoom)
	{
		OriginalLagSpeed = FollowBoom->CameraLagSpeed;
		OriginalLagRotationSpeed = FollowBoom->CameraRotationLagSpeed;

		FollowBoom->bUsePawnControlRotation = false;

		FollowBoom->CameraLagSpeed = 1.0f;
		FollowBoom->CameraRotationLagSpeed = 1.0f;
	}
}

void CameraStateSlowDown::OnExit(APlatformerCameraManager* CameraManager)
{
	USpringArmComponent* FollowBoom = CameraManager->GetFollowBoom();

	if (FollowBoom)
	{
		FollowBoom->bUsePawnControlRotation = true;

		FollowBoom->CameraLagSpeed = OriginalLagSpeed;
		FollowBoom->CameraRotationLagSpeed = OriginalLagSpeed;
		FollowBoom->bDoCollisionTest = true;
		FollowBoom->ProbeSize = 12.0f;
	}
}

void CameraStateSlowDown::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{

}
////////////////////////////////////////////////////////////////////////////

/////////////////////////////CAMERA DYING ////////////////////////////////
void CameraStateDying::OnEnter(APlatformerCameraManager* CameraManager)
{
	UCameraComponent* FollowCamera = CameraManager->GetFollowCamera();
	UCameraComponent* DeathCamera = CameraManager->GetDeathCamera();

	if(IsValid(FollowCamera))
		FollowCamera->SetActive(false);

	if(IsValid(DeathCamera))
		DeathCamera->SetActive(true);
}

void CameraStateDying::OnExit(APlatformerCameraManager* CameraManager)
{
	UCameraComponent* FollowCamera = CameraManager->GetFollowCamera();
	if(IsValid(FollowCamera))
		FollowCamera->SetActive(true);

	UCameraComponent* DeathCamera = CameraManager->GetDeathCamera();
	if(IsValid(DeathCamera))
		DeathCamera->SetActive(false);
}

void CameraStateDying::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{
	APlatformCharacter* PlatformCharacter = CameraManager->GetPlatformCharacter();
	UCameraComponent* DeathCamera = CameraManager->GetDeathCamera();

	if (PlatformCharacter == nullptr || DeathCamera == nullptr) return;

	//Update DeathCamera
	FVector ActorLocation = PlatformCharacter->GetActorLocation();

	FVector DeathCameraLocation = DeathCamera->GetComponentLocation();
	//DeathCamera->SetWorldLocation(FVector(ActorLocation.X, ActorLocation.Y, DeathCameraLocation.Z));
}
/////////////////////////////////////////////////////////////////////////


///////////////////////CAMERA SPEED UP /////////////////////////////////
void CameraStateSpeedUp::OnEnter(APlatformerCameraManager* CameraManager)
{
	UCameraComponent* CameraComponent = CameraManager->GetFollowCamera();
	USpringArmComponent* FollowBoom = CameraManager->GetFollowBoom();

	if (FollowBoom)
	{
		FollowBoom->TargetArmLength /= 2;
		FollowBoom->CameraLagSpeed *= 2;
		FollowBoom->CameraRotationLagSpeed *= 2;
	}

	if(CameraComponent && CameraComponent->PostProcessSettings.WeightedBlendables.Array.Num() > 0)
		CameraComponent->PostProcessSettings.WeightedBlendables.Array[0].Weight = 1.0f;
}

void CameraStateSpeedUp::OnExit(APlatformerCameraManager* CameraManager)
{
	UCameraComponent* CameraComponent = CameraManager->GetFollowCamera();
	USpringArmComponent* FollowBoom = CameraManager->GetFollowBoom();
	
	if (FollowBoom)
	{
		FollowBoom->TargetArmLength *= 2;
		FollowBoom->CameraLagSpeed /= 2;
		FollowBoom->CameraRotationLagSpeed /= 2;
	}

	if(CameraComponent && CameraComponent->PostProcessSettings.WeightedBlendables.Array.Num() > 0)
		CameraComponent->PostProcessSettings.WeightedBlendables.Array[0].Weight = 0.0f;
}

void CameraStateSpeedUp::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{
}
////////////////////////////////////////////////////////////////////////

////////////////////////////CAMERA AIMING///////////////////////////////

void CameraStateAiming::OnEnter(APlatformerCameraManager* CameraManager)
{
	UCameraComponent* CameraComponent = CameraManager->GetFollowCamera();
	USpringArmComponent* FollowBoom = CameraManager->GetFollowBoom();
}

void CameraStateAiming::OnExit(APlatformerCameraManager* CameraManager)
{
	UCameraComponent* CameraComponent = CameraManager->GetFollowCamera();
	USpringArmComponent* FollowBoom = CameraManager->GetFollowBoom();
}

void CameraStateAiming::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{

}

////////////////////////////////////////////////////////////////////

//////////////////////////CAMERA IDLE///////////////////////////////

void CameraStateIdle::OnEnter(APlatformerCameraManager* CameraManager)
{
}

void CameraStateIdle::OnExit(APlatformerCameraManager* CameraManager)
{
}

void CameraStateIdle::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{
	IdleTimer += DeltaTime;

	APlatformCharacter* Character = CameraManager->GetPlatformCharacter();

	if (Character)
	{
		Character->AddControllerYawInput(DeltaTime * 10.0f);
		CameraManager->GetFollowBoom()->TargetArmLength = 400.0f;
	}
}

///////////////////////////////////////////////////////////////////

void CameraStateStarting::SetCameraSettings(FCameraAnimSettings NewSettings)
{
	Settings = NewSettings;
}

//////////////////////////CAMERA STARTING///////////////////////////////

void CameraStateStarting::OnEnter(APlatformerCameraManager* CameraManager)
{
	APlatformCharacter* Character = CameraManager->GetPlatformCharacter();
	USpringArmComponent* ArmComponent = CameraManager->GetFollowBoom();


	if (ArmComponent)
		ArmComponent->TargetArmLength = Settings.ArmLength;

	if (Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->Controller);

		Character->AddControllerYawInput(Settings.CameraYawRotationSpeed * Settings.StartingCameraAnimationTimer * -2);
		Character->AddControllerPitchInput(Settings.CameraPitchRotationSpeed * Settings.StartingCameraAnimationTimer);
		if(PlayerController)
			Character->DisableInput(PlayerController);
	}

	TimerCounter = 0;
}

void CameraStateStarting::OnExit(APlatformerCameraManager* CameraManager)
{
	APlatformCharacter* Character = CameraManager->GetPlatformCharacter();
	if (Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->Controller);
		if(PlayerController)
			Character->EnableInput(PlayerController);
	}
}

void CameraStateStarting::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{
	APlatformCharacter* Character = CameraManager->GetPlatformCharacter();
	USpringArmComponent* ArmComponent = CameraManager->GetFollowBoom();

	if (!Character || !ArmComponent) return;

	Character->AddControllerYawInput(Settings.CameraYawRotationSpeed * DeltaTime * -1);
	Character->AddControllerPitchInput(Settings.CameraPitchRotationSpeed * DeltaTime * -1);
	
	ArmComponent->TargetArmLength -= Settings.ArmReductionSpeed * DeltaTime;
	ArmComponent->TargetArmLength = FMath::Clamp(ArmComponent->TargetArmLength, 100.0f, ArmComponent->TargetArmLength);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////CAMERA MAP CLEAR///////////////////////////////

void CameraStateEnding::SetCameraSettings(FCameraAnimSettings NewSettings)
{
	Settings = NewSettings;
}

void CameraStateEnding::OnEnter(APlatformerCameraManager* CameraManager)
{
	APlatformCharacter* Character = CameraManager->GetPlatformCharacter();
	USpringArmComponent* ArmComponent = CameraManager->GetFollowBoom();

	if (!Character || !ArmComponent) return;
	
	APlayerController* PlayerController = Cast<APlayerController>(Character->Controller);
	if(PlayerController)
		Character->DisableInput(PlayerController);

	Character->AddControllerYawInput(Settings.CameraYawRotationSpeed * Settings.StartingCameraAnimationTimer * 2);
	Character->AddControllerPitchInput(Settings.CameraPitchRotationSpeed * Settings.StartingCameraAnimationTimer * -1);
}

void CameraStateEnding::OnExit(APlatformerCameraManager* CameraManager)
{
	APlatformCharacter* Character = CameraManager->GetPlatformCharacter();
	if (Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->Controller);
		if(PlayerController)
			Character->EnableInput(PlayerController);
	}
}

void CameraStateEnding::OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager)
{
	APlatformCharacter* Character = CameraManager->GetPlatformCharacter();
	USpringArmComponent* ArmComponent = CameraManager->GetFollowBoom();

	if (!Character || !ArmComponent) return;

	Character->AddControllerYawInput(Settings.CameraYawRotationSpeed * DeltaTime);
	Character->AddControllerPitchInput(Settings.CameraPitchRotationSpeed * DeltaTime);
	
	ArmComponent->TargetArmLength += Settings.ArmReductionSpeed * DeltaTime;
	ArmComponent->TargetArmLength = 
		FMath::Clamp(ArmComponent->TargetArmLength, ArmComponent->TargetArmLength, Settings.ArmLength);
}
///////////////////////////////////////////////////////////////////
