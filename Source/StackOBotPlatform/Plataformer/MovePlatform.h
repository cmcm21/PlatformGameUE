// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovePlatform.generated.h"

class UStaticMesh;

UENUM(Meta = (UseEnumValuesAsMaskValuesInEditor = "true"))
enum class MoveFunction { SIN=0x01, COS=0x02, NONE=0x00};

ENUM_CLASS_FLAGS(MoveFunction)

UCLASS()
class STACKOBOTPLATFORM_API AMovePlatform : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AMovePlatform();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	FVector startLocation;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fixed Move")
	bool isFixedMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fixed Move")
	FVector moveDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fixed Move")
	float fixedMoveSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fixed Move")
	float moveLimit = 30.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MoveTransform", Meta = (Bitmask, BitmaskEnum = "MoveFunction"))
	int32 xMoveMethod;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MoveTransform", Meta = (Bitmask, BitmaskEnum = "MoveFunction"))
	int32 yMoveMethod;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MoveTransform", Meta = (Bitmask, BitmaskEnum = "MoveFunction"))
	int32 zMoveMethod;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float amplitude;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float timeScaled;

	UPROPERTY(VisibleAnywhere)
	UMeshComponent* platformMesh;
	
	UFUNCTION(BlueprintPure)
	float TransformCos();

	UFUNCTION(BlueprintPure)
	float TransformSin();

	float runningTime;
private:
	FTimerHandle timerHandle;
	float changeTime = 0;

	float MoveOnXAxe();
	float MoveOnYAxe();
	float MoveOnZAxe();

	void FixedMove(float deltaTime);
	float timerSpeed = 0.01f;
};
