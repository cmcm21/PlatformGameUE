// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StackOBotPlatform/Character/PlatformCameraTypes.h" //ECameraState
#include "CoreMinimal.h"


//TODO: Rotate Camera after several second on the idle state
class CameraStateMovement : public ICameraStateInterface
{

public:
	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;

private:
	float CameraMoveInAir();
	APlatformCharacter* PlatformCharacter;
};


class CameraStateSlowDown : public ICameraStateInterface
{

public:
	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;

private:
	float OriginalLagSpeed;
	float OriginalLagRotationSpeed;
};


class CameraStateDying : public ICameraStateInterface
{

public:
	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;
};


class CameraStateSpeedUp : public ICameraStateInterface
{

public:
	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;
};


class CameraStateAiming : public ICameraStateInterface
{

public:
	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;
};

class CameraStateStarting : public ICameraStateInterface
{

public:
	void SetCameraSettings(FCameraAnimSettings Settings);

	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;

private:
	float TimerCounter = 0;
	FCameraAnimSettings Settings;
};


class CameraStateEnding : public ICameraStateInterface
{

public:
	void SetCameraSettings(FCameraAnimSettings Settings);

	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;

private:
	FCameraAnimSettings Settings;
};


class CameraStateIdle : public ICameraStateInterface
{

public:
	virtual void OnEnter(APlatformerCameraManager* CameraManager) override;
	virtual void OnExit(APlatformerCameraManager* CameraManager) override;
	virtual void OnUpdate(float DeltaTime, APlatformerCameraManager* CameraManager) override;

private:
	float IdleTimer = 0;
	float CameraRotationSpeed = 10.0f;

};


