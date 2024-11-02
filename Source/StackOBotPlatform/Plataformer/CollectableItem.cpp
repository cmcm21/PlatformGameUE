// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItem.h"
#include "Components/StaticMeshComponent.h"
#include "StackOBotPlatform/Character/PlatformCharacter.h"
#include "PlatformGameMode.h"
#include "UObject/UnrealType.h"
#include "Kismet/GameplayStatics.h"

#include "Subsystems/OrbsEffectManagerSubsystem.h"
#include "Subsystems/InventoryManagerSubsystem.h"

void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();
	gameMode = Cast<APlatformGameMode>(GetWorld()->GetAuthGameMode());
	orbData = orbDataRowHandle.GetRow<FOrbDataStruct>(TEXT("Orbs data struct"));
}

// Sets default values
ACollectableItem::ACollectableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = ACollectableItem::CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(mesh);

	mesh->OnComponentBeginOverlap.AddDynamic(this, &ACollectableItem::OnBeginOverlap);
}

void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	runningTime += DeltaTime;

	AddActorWorldOffset(FVector(0, 0, SinTransform()));
}

void ACollectableItem::SetVisibility(bool visibility)
{
	if (mesh)
		mesh->SetVisibility(visibility);
}

#if WITH_EDITOR
void ACollectableItem::PostEditChangeProperty(FPropertyChangedEvent& propertyEvent)
{
	Super::PostEditChangeProperty(propertyEvent);

	const static FName rowName = FName("RowName");
	if (propertyEvent.ChangeType == EPropertyChangeType::ValueSet && propertyEvent.GetPropertyName() == rowName)
	{
		
		orbData = orbDataRowHandle.GetRow<FOrbDataStruct>(TEXT("Orbs data struct"));
		if (mesh != nullptr && orbData != nullptr)
			mesh->SetMaterial(0, orbData->orbMaterial);
	}
}
#endif

void ACollectableItem::OnBeginOverlap( UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlatformCharacter *otherCharacter = Cast<APlatformCharacter>(OtherActor);
	if (otherCharacter)
	{
		character = otherCharacter;
		Collected();
	}
}

float ACollectableItem::SinTransform()
{
	float sin = FMath::Sin(runningTime * timeScaled);
	return amplitude * sin;
}

void ACollectableItem::GetOrbSubsystem()
{
	if (orbsSubsys) return;

	UWorld* world = GetWorld();
	if (world)
		orbsSubsys = world->GetSubsystem<UOrbsEffectManagerSubsystem>();
}

void ACollectableItem::GetInventorySubsystem()
{
	if (inventoryManagerSubsys) return;
	UWorld* world = GetWorld();
	if (world)
		inventoryManagerSubsys = world->GetSubsystem<UInventoryManagerSubsystem>();
}

void ACollectableItem::Collected_Implementation()
{
	mesh->SetVisibility(false);

	if(character) character->UpdateExp();
	if (gameMode) gameMode->ItemCollected(this);
	//if (gameMode && orbData) gameMode->ApplyEffect(*orbData);
	GetOrbSubsystem();
	GetInventorySubsystem();
	if (orbData == nullptr) return;

	if (orbsSubsys != nullptr)
		orbsSubsys->CollectOrb(*orbData);

	if (inventoryManagerSubsys != nullptr && orbData->effectData.effectType == FOrbEffectType::ORB_POSSITIVE)
		inventoryManagerSubsys->AddOrb(*orbData);
}



