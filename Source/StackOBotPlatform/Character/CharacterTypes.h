#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_JUMPING UMETA(DisplayName="Jumpling"),
	ECS_DYING UMETA(DisplayName="Dying"),
	ECS_AIMING UMETA(DisplayName="Aiming"),
	ECS_IDLE UMETA(DisplayName="Idle"),
	ECS_CANNOTJUMP UMETA(DisplayName="NoJump")
};


