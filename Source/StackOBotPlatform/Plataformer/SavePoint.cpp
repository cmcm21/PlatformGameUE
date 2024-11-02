// Fill out your copyright notice in the Description page of Project Settings.
#include "SavePoint.h"
#include "Components/StaticMeshComponent.h"
#include "StackOBotPlatform/Character/PlatformCharacter.h"
#include "PlatformGameMode.h"
#include "Components/SceneComponent.h"
#include "HUD/PlatformHUD.h"

void ASavePoint::BeginPlay()
{
	GetGameMode();
	GetHUD();
}

// Sets default values
ASavePoint::ASavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

void ASavePoint::OnPlayerHit()
{
	if (!gameMode || !HUD) return;

	gameMode->StoreSavePoint(GetActorLocation() + FVector(0,0,200.0f));
}

void ASavePoint::OnPlayerEnter()
{
	if (!gameMode || !HUD) return;

	gameMode->OnEnterSaveArea();
	HUD->ShowWidget(HUDWidgetID::HUD_SAVE_W);
}

void ASavePoint::OnPlayerExit()
{
	if(!gameMode || !HUD) return;

	gameMode->OnExitSaveArea();
	HUD->HideWidget(HUDWidgetID::HUD_SAVE_W);
}

void ASavePoint::GetGameMode()
{
	if(!gameMode) gameMode = Cast<APlatformGameMode>(GetWorld()->GetAuthGameMode());
}

void ASavePoint::GetHUD()
{
	if (!HUD)
	{
		UWorld* world = GetWorld();
		if (!world) return;
		auto firstPlayerController = world->GetFirstPlayerController();

		if (!firstPlayerController) return;
		HUD = Cast<APlatformHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	}
}

