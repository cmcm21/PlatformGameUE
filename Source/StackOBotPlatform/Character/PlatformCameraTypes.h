#pragma once
#include "PlatformCameraTypes.generated.h"

class APlatformerCameraManager;
class UCameraComponent;
class APlatformCharacter;

USTRUCT(BlueprintType)
struct FCameraAnimSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float StartingCameraAnimationTimer = 7.5f;

	UPROPERTY(EditAnywhere)
	float CameraYawRotationSpeed = 10.0f;

	UPROPERTY(EditAnywhere)
	float CameraPitchRotationSpeed = 4.5f;

	UPROPERTY(EditAnywhere)
	float ArmLength = 1000.0f;

	UPROPERTY(EditAnywhere)
	float ArmReductionSpeed = 20.0f;
};



UENUM(BlueprintType)
enum class ECameraManagerState : uint8
{
	ECMS_SLOWTIME UMETA(DisplayName="Slow time"),
	ECMS_IDLE UMETA(DisplayName="Idle"),
	ECMS_DIYING UMETA(DisplayName="Diying"),
	ECMS_STARTING UMETA(DisplayName="Starting"),
	ECMS_ENDING UMETA(DisplayName="Ending")
};

UINTERFACE(MinimalAPI)
class UCameraStateInterface : public UInterface
{
	GENERATED_BODY()
};

class ICameraStateInterface
{
	GENERATED_BODY()

public:
	virtual void OnEnter(APlatformerCameraManager* CameraManager) = 0;
	virtual void OnExit(APlatformerCameraManager* CameraManager) = 0;
	virtual void OnUpdate(float DeltaTime,APlatformerCameraManager* CameraManager) = 0;
};

