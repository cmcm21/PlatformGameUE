// Fill out your copyright notice in the Description page of Project Settings.
#include "CannonProjectile.h"

// Sets default values
ACannonProjectile::ACannonProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACannonProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACannonProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

