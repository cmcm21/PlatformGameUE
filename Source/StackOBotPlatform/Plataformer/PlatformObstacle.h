// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformObstacle.generated.h"

class UStaticMeshComponent;

UCLASS()
class STACKOBOTPLATFORM_API APlatformObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	APlatformObstacle();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation")
	FVector rotateVelocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Roation")
	float rotationSpeed;
	
public:	
	virtual void Tick(float DeltaTime) override;
	void Rotate(float deltaTime);
};
