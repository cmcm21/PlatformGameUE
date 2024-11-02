// Fill out your copyright notice in the Description page of Project Settings.
#include "MovePlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SplineComponent.h"

// Sets default values
AMovePlatform::AMovePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	platformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	platformMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AMovePlatform::BeginPlay()
{
	Super::BeginPlay();
	startLocation = GetActorLocation();
}

float AMovePlatform::TransformCos()
{
	return amplitude * FMath::Cos(runningTime * timeScaled);
}

float AMovePlatform::TransformSin()
{
	return amplitude * FMath::Sin(runningTime * timeScaled);
}

float AMovePlatform::MoveOnXAxe()
{
	float calculatedValue;
	if (xMoveMethod == (int32)MoveFunction::COS)
		calculatedValue =  TransformCos();
	else if (xMoveMethod == (int32)MoveFunction::SIN)
		calculatedValue =  TransformSin();
	else
		calculatedValue = 0;

	return calculatedValue;
}

float AMovePlatform::MoveOnYAxe()
{
	float calculatedValue;
	if (yMoveMethod == (int32)MoveFunction::COS)
		calculatedValue =  TransformCos();
	else if (yMoveMethod == (int32)MoveFunction::SIN)
		calculatedValue =  TransformSin();
	else
		calculatedValue = 0;

	return calculatedValue;
}

float AMovePlatform::MoveOnZAxe()
{
	float calculatedValue;
	if (zMoveMethod == (int32)MoveFunction::COS)
		calculatedValue =  TransformCos();
	else if (zMoveMethod == (int32)MoveFunction::SIN)
		calculatedValue =  TransformSin();
	else
		calculatedValue = 0;

	return calculatedValue;
}

void AMovePlatform::FixedMove(float deltaTime)
{
	FVector actorLocation = GetActorLocation();
	actorLocation += moveDirection * deltaTime * fixedMoveSpeed;

	SetActorLocation(actorLocation);

	float distance = FVector::Distance(startLocation, actorLocation);
	if (distance > moveLimit && changeTime > 1)
	{
		moveDirection *= -1;
		changeTime = 0;
	}
}

// Called every frame
void AMovePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFixedMove)
	{
		changeTime += DeltaTime;
		FixedMove(DeltaTime);
	}
	else 
	{
		runningTime += DeltaTime;
		FVector movement = FVector(MoveOnXAxe(), MoveOnYAxe(), MoveOnZAxe());
		AddActorWorldOffset(movement);
	}
}

































	//if (!isFixedMove)
	//{
	
	//}
	//else 
	/*{
		FVector movement = FixedMove(DeltaTime);
		movement += GetActorLocation();
		SetActorLocation(movement);
	}*/

