// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CameraStates/CameraStates.h"
#include "PlatformerCameraManager.generated.h"

class UCameraComponent;
class APlatformCharacter;
class UPlatformerCameraShake;
class ICameraStateInterface;
class CameraStateStarting;
class USpringArmComponent;
//struct FCameraStartingSettings;
/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API APlatformerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void UpdateCamera(float DeltaTime) override;

	FORCEINLINE void SetCharacterFollowCamera(UCameraComponent* Camera) { CharacterFollowCamera = Camera; }
	FORCEINLINE UCameraComponent* GetFollowCamera() { return CharacterFollowCamera; }

	FORCEINLINE void SetFollowCameraBoom(USpringArmComponent* BoomComponent) { FollowCameraBoom = BoomComponent; }
	FORCEINLINE USpringArmComponent* GetFollowBoom() { return FollowCameraBoom; }

	FORCEINLINE void SetCharacterDeathCamera(UCameraComponent* Camera) { CharacterDeathCamera = Camera; }
	FORCEINLINE UCameraComponent* GetDeathCamera() { return CharacterDeathCamera; }

	FORCEINLINE void SetCharacterWinCamera(UCameraComponent* Camera) { CharacterWinCamera = Camera; }
	FORCEINLINE UCameraComponent* GetWinCamera() { return CharacterWinCamera; }

	FORCEINLINE void SetCharacter(APlatformCharacter* PlatformCharacter) { Character = PlatformCharacter; }
	FORCEINLINE APlatformCharacter* GetPlatformCharacter() { return Character; }

	FORCEINLINE ICameraStateInterface* GetState() { return CameraState; }
	FORCEINLINE FCameraAnimSettings GetCameraStartingSettings() { return CameraStartingSettings; }

	UFUNCTION()
	void Shake(float duration=0.5f);

	UFUNCTION()
	void StopShake();
	void ChangeState(ICameraStateInterface* NewState);

	UFUNCTION(BlueprintCallable)
	void RunStartingCamera();
	
	UFUNCTION(BlueprintCallable)
	void RunEndingCamera();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRunStartingCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRunEndingCamera;

	UPROPERTY()
	USpringArmComponent* FollowCameraBoom;

	UPROPERTY()
	UCameraComponent* CharacterFollowCamera;

	UPROPERTY()
	UCameraComponent* CharacterDeathCamera;

	UPROPERTY()
	UCameraComponent* CharacterWinCamera;

	UPROPERTY()
	APlatformCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPlatformerCameraShake> CameraShake;

	UPROPERTY()
	FTimerHandle TimeHandler;

	UPROPERTY(EditAnywhere)
	FCameraAnimSettings CameraStartingSettings;

	UPROPERTY(EditAnywhere)
	FCameraAnimSettings CameraEndingSettings;

	CameraStateStarting* CameraStartingState;

	CameraStateEnding* CameraEndingState;

	ICameraStateInterface* CameraState;

private:
	void OnTimerEndCallback();

	float IdleTimer = 0;
	float StartingCameraTimer = 0;

	ECameraManagerState State;
};
