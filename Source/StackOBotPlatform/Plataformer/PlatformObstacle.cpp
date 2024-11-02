// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"


// Sets default values
APlatformObstacle::APlatformObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlatformObstacle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlatformObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate(DeltaTime);
}

void APlatformObstacle::Rotate(float deltaTime)
{
	FRotator Rotator = rotateVelocity.Rotation();
	FRotator ActorRotation = GetActorRotation();
	ActorRotation += Rotator * deltaTime * rotationSpeed;

	SetActorRotation(ActorRotation, ETeleportType::TeleportPhysics);
}

