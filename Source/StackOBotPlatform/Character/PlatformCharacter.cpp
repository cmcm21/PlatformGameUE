#include "PlatformCharacter.h"
#include "StackOBotPlatform/Plataformer/CannonProjectile.h"

//Enhanced Input libraries
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "InputTriggers.h"//UInputTrigger

//Character components libraries
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"

//#include "Components/SkeletalMeshComponent.h/"
#include "GameFramework/Actor.h"
#include "GameFramework/Actor.h"

//Camera Libraries
#include "Camera/PlayerCameraManager.h"
#include "PlatformCameraTypes.h"

//Game Data libraries
#include "StackOBotPlatform/Plataformer/PlatformGameMode.h"
#include "Engine/DataTable.h"
#include "StackOBotPlatform/GameModeData.h"

//HUD Libraries
#include "HUD/PlatformHUD.h"
#include "StackOBotPlatform/Plataformer/HUDManager.h"

//Subsystems libraries
#include "Subsystems/InputManagerSubsystem.h"

//Utils
//To calculate Projectile Spawn Position With LineTrace
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h" 
#include "DrawDebugHelpers.h"

//Other Platform Game classes libraries
#include "StackOBotPlatform/Plataformer/PlatformObstacle.h"
#include "StackOBotPlatform/Plataformer/PlatformerProjectile.h"
#include "StackOBotPlatform/Character/PlatformerCameraManager.h"
#include "StackOBotPlatform/Character/CameraStates/CameraStates.h" //CameraStateIdle,CameraStateDeath,etc...
#include "StackOBotPlatform/Character/PlatformPlayerController.h"
#include "StackOBotPlatform/GameModeData.h"
#include "Weapons/Weapon.h"


// Sets default values
APlatformCharacter::APlatformCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//The character don't rotate to any direction when the controller rotates. 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	InitComponents();

	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	if (characterMovement)
	{
		//set the character rotate to the controller orientation
		characterMovement->bOrientRotationToMovement = true;
		//Set the rotation speed for the Z axis -> Yaw Rotator
		characterMovement->RotationRate = FRotator(0.f, 300.f, 0.f);
	}
}

//called in constructor, create/init necessary components for the character
void APlatformCharacter::InitComponents()
{
	BoomCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("BoomCamera"));
	BoomCamera->SetupAttachment(GetRootComponent());
	BoomCamera->TargetArmLength = 600.0f;
	//we don't want that our camera collide againts nothing.
	BoomCamera->ProbeSize = 0;
	//rotate camera acording  to the control rotation of the character/pawn
	BoomCamera->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FollowCamera->SetupAttachment(BoomCamera);

	//Death Camera setup
	DeathCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DeathCameraComponent"));
	DeathCamera->SetupAttachment(GetRootComponent());
	DeathCamera->bAutoActivate = false;
	DeathCamera->SetActive(false);
	
	UCapsuleComponent* CharacterCapsuleComponent = GetCapsuleComponent();
	if (CharacterCapsuleComponent)
	{
		CharacterCapsuleComponent->SetGenerateOverlapEvents(true);
		CharacterCapsuleComponent->OnComponentHit.AddDynamic(this, &APlatformCharacter::OnCapsuleHit);
	}
}

// Called when the game starts or when spawned
void APlatformCharacter::BeginPlay()
{
	Super::BeginPlay();
	BindNeededActors();
	SetupCameraComponents();
	InitWeapon();

	bBlockInput = false;
	CharacterState = ECharacterState::ECS_IDLE;
	
	AttachEnhancedInput();

	GetHUD();
	if (HUD) HUD->ShowCrosshair(false);
}

void APlatformCharacter::BindNeededActors()
{
	GameMode = Cast<APlatformGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode)
		GameMode->OnGameoverEvent().AddUObject(this, &APlatformCharacter::OnGameoverCallback);
}

void APlatformCharacter::SetupCameraComponents()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlatformPlayerController* PlatformPlayerController = Cast<APlatformPlayerController>(World->GetFirstPlayerController());

		if (PlatformPlayerController)
		{
			PlatformerCameraManager = PlatformPlayerController->GetCameraManager();

			PlatformerCameraManager->SetCharacterDeathCamera(DeathCamera);
			PlatformerCameraManager->SetCharacterFollowCamera(FollowCamera);
			PlatformerCameraManager->SetFollowCameraBoom(BoomCamera);
			PlatformerCameraManager->SetCharacter(this);
			PlatformerCameraManager->ChangeState(new CameraStateMovement);
		}
	}
}

void APlatformCharacter::InitWeapon()
{
	if (WeaponClass)
	{
		FTransform SpawnTransform = GetMesh()->GetSocketTransform(FName("RightHandSocket"),ERelativeTransformSpace::RTS_World);
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnTransform);
		Weapon->Equip(GetMesh(), FName("RightHandSocket"));

		if (Weapon) Weapon->Hide(true);
	}
}
void APlatformCharacter::AttachEnhancedInput()
{
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem *Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
			//we set the mapping context with the highest priority that is 0
			Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void APlatformCharacter::OnGameoverCallback()
{
	if (GameMode->GetGameState() == EGameModeState::GS_WIN && PlatformerCameraManager)
		PlatformerCameraManager->RunEndingCamera(); 

	if (GameMode->GetGameState() == EGameModeState::GS_LOSE)
		Destroy();
	else
	{
		GetCharacterMovement()->DisableMovement();
		BoomCamera->bUsePawnControlRotation = false;
	}
}

void APlatformCharacter::SetCurrentDataTableRow(FLevelingInformationTable *data)
{
	PlayerData.level = data->level;
	NextLevelExp = data->exp;
	MaxJumps = data->jumps;

	ConfigurateJumps();
}

void APlatformCharacter::ConfigurateJumps()
{
	JumpMaxCount = MaxJumps;

	if (!GetCharacterMovement()) return;
	GetCharacterMovement()->MaxJumpApexAttemptsPerSimulation = MaxJumps;
	GetCharacterMovement()->JumpZVelocity = PlayerData.jumpHigh;

	//In order to can test without the PlatformGameMode
	if (GetCharacterMovement()->JumpZVelocity <= 0)
		GetCharacterMovement()->JumpZVelocity = 800.f;
}

void APlatformCharacter::GetHUD()
{
	if(GetWorld() && GetWorld()->GetFirstLocalPlayerFromController())
		HUD = Cast<APlatformHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void APlatformCharacter::MoveForward(float value)
{
	//get the current rotation
	FRotator rotation = GetControlRotation();
	//calculate the forward vector based in the rotation
	FVector Forward = UKismetMathLibrary::GetForwardVector(FRotator(0,rotation.Yaw, 0));
	//call the built-in AddMovementInput method passing the direction for move and the scaledValue
	AddMovementInput(Forward, value);
}

void APlatformCharacter::MoveRight(float value)
{
	//get the current control rotation
	FRotator rotation = GetControlRotation();
	//get the right vector based in the rotation
	FVector Right = UKismetMathLibrary::GetRightVector(FRotator(0,rotation.Yaw, rotation.Roll));
	//call the built-in AddMovementInput method passing the direction for move and the scaledValue
	AddMovementInput(Right, value);
}

void APlatformCharacter::Die()
{
	if (CharacterState == ECharacterState::ECS_DYING) return;

	CharacterState = ECharacterState::ECS_DYING;
	USkeletalMeshComponent* SkeletalMesh = GetMesh();

	if (SkeletalMesh)
	{
		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	bIsUnderOrbEffect = false;

	if (PlatformerCameraManager && Cast<CameraStateDying>(PlatformerCameraManager->GetState()) == nullptr)
		PlatformerCameraManager->ChangeState(new CameraStateDying);

	FTimerHandle WaitWhileShowingDiedW;
	GetWorld()->GetTimerManager().ClearTimer(WaitWhileShowingDiedW);
	GetWorld()->GetTimerManager().SetTimer(WaitWhileShowingDiedW, [&]() {

			if(HUD != nullptr)
				HUD->ToggleWidget(HUDWidgetID::HUD_DIED_W);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&APlatformCharacter::Destroyed, FallingTime, false);

		},1.5f,false);
}

void APlatformCharacter::ApplyOrbEffect(FOrbDataStruct OrbData)
{
	if (OrbData.orbID == FOrbID::ORB_SlowDown)
		if (PlatformerCameraManager)
			PlatformerCameraManager->ChangeState(new CameraStateSlowDown);

	if(OrbData.orbID == FOrbID::ORB_SpeedUp)
		if (PlatformerCameraManager)
			PlatformerCameraManager->ChangeState(new CameraStateSpeedUp);

	bIsUnderOrbEffect = true;
}

void APlatformCharacter::DiscardOrbEffect(FOrbDataStruct OrbData)
{
	if(OrbData.orbID == FOrbID::ORB_SlowDown || OrbData.orbID == FOrbID::ORB_SpeedUp)
		if (PlatformerCameraManager)
			PlatformerCameraManager->ChangeState(new CameraStateMovement);

	bIsUnderOrbEffect = false;
}

void APlatformCharacter::HandleMoveAction(const FInputActionValue& value)
{
	//we skip if the location is locked
	if (CharacterState == ECharacterState::ECS_DYING || CharacterState == ECharacterState::ECS_AIMING) return;
	//Get the directions of the input action value
	FVector2D direction = value.Get<FVector2D>();

	if (PlatformerCameraManager)
		PlatformerCameraManager->ChangeState(new CameraStateMovement);

	//pass the Horizontal (X) value of the vector to the MoveRight function
	MoveRight(direction.X);
	//pass the Vertical (Y) value of the vector to the MoveForward function
	MoveForward(direction.Y);
}

void APlatformCharacter::HandleJumpAction(const FInputActionValue& value)
{
	//we skip if the location is lock or the character is unable to jump
	if ( CharacterState == ECharacterState::ECS_AIMING ||
		CharacterState == ECharacterState::ECS_DYING ||
		!bCanJump) return;

	//we transform the input action value to bool
	bool keyPressed = value.Get<bool>();
	if(keyPressed)
	{ 
		//If character is on ground then reset the value of jump counter to zero
		if (!GetCharacterMovement()->IsFalling())
		{
			jumpsCounter = 0;
		}

		//we check that character hasn't passed the max number of Jumps
		if (jumpsCounter <= MaxJumps)
		{
			//this variable trigger the character's jump
			CharacterState = ECharacterState::ECS_JUMPING;
			bPressedJump = keyPressed;
			jumpsCounter++;
		}
	}
	else 
		//if key is false, then we stop to jump.
		bPressedJump = keyPressed;
}

void APlatformCharacter::HandleLookAction(const FInputActionValue& value)
{
	//if (IsLockLocation() || bInInventory || CharacterState == ECharacterState::ECS_DYING) return;
	if (bInInventory || CharacterState == ECharacterState::ECS_DYING) return;
	FVector2D direction = value.Get<FVector2D>();

	if (PlatformerCameraManager)
		PlatformerCameraManager->ChangeState(new CameraStateMovement);

	AddControllerYawInput(direction.X);
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	AddControllerPitchInput(direction.Y);
}

void APlatformCharacter::HandleSaveAction(const FInputActionValue& value)
{	
	if (GameMode->GetGameState() != EGameModeState::GS_SAVE_AREA) return;

	if (HUD == nullptr)
		GetHUD();

	if (!HUD) return;
	if (PlatformerCameraManager) PlatformerCameraManager->ChangeState(new CameraStateMovement);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetWorld()->GetFirstPlayerController());
	HUD->PushMenuToHUD(HUDMenuID::HUD_SAVE_M);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void APlatformCharacter::HandleInventoryAction(const FInputActionValue& value)
{
	if (CharacterState != ECharacterState::ECS_IDLE) return;

	if (PlatformerCameraManager) PlatformerCameraManager->ChangeState(new CameraStateMovement);

	//actually we don't care what value the inputAction has, we just need to know if the action was triggered or it wasn't
	ToggleInventory();
}

void APlatformCharacter::ToggleInventory()
{
	if (CharacterState != ECharacterState::ECS_IDLE) return;
	//if we haven't get the hud class then we request it
	if (HUD == nullptr)
		GetHUD();

	if (!HUD) return;
	//We pass the widget id of the inventory to show it on HUD
	HUD->ToggleWidget(HUDWidgetID::HUD_INVENTOR_W);
	//We keep a reference to know if the inventory is showing or hidding in order to prevent 
	//show the inventory when the player is restarted.
	bInInventory = !bInInventory;
}

void APlatformCharacter::HandleShortcutAction(const FInputActionValue& value)
{
	if (CharacterState == ECharacterState::ECS_DYING || CharacterState == ECharacterState::ECS_JUMPING) return;
	//We get the float value of the shortcut pressed
	int32 key = (int32)value.Get<float>();

	if (PlatformerCameraManager) PlatformerCameraManager->ChangeState(new CameraStateMovement);
	//request the Input Manager system and trigger short cut event passing the shortcut key as an argument
	GetInputManagerSubsys(); return;
	if (InputManagerSubsys)
		InputManagerSubsys->TriggerShortCutEvent(key);
}

void APlatformCharacter::HandleFireAction(const FInputActionValue& value)
{
	if (CharacterState != ECharacterState::ECS_AIMING || !ProjectileClass || !bCanFire) return;
	PlayFireAnimationEvent();
	bCanFire = false;
}

void APlatformCharacter::HandleChangeCombatAction(const FInputActionValue& value)
{
	if (CharacterState == ECharacterState::ECS_AIMING 
		|| CharacterState != ECharacterState::ECS_IDLE) return;

	CharacterState = 
		CharacterState == ECharacterState::ECS_AIMING 
		? ECharacterState::ECS_IDLE 
		: ECharacterState::ECS_AIMING;

	bUseControllerRotationYaw = CharacterState == ECharacterState::ECS_AIMING;
	if (!HUD) GetHUD();

	HUD->ShowCrosshair(CharacterState == ECharacterState::ECS_AIMING);

	if (CharacterState == ECharacterState::ECS_IDLE)
	{
		GetCapsuleComponent()->SetRelativeRotation(BeforeCameraViewChangeRotation);
		BoomCamera->SetRelativeLocation(FVector(0, 0, 0));

		if(PlatformerCameraManager)
			PlatformerCameraManager->ChangeState(new CameraStateMovement());

		if (Weapon) Weapon->Hide(true);
	}
	else if (CharacterState == ECharacterState::ECS_AIMING)
	{
		BeforeCameraViewChangeRotation = GetCapsuleComponent()->GetRelativeRotation();
		BoomCamera->SetRelativeLocation(CombatBoomCameraOffset);

		if(PlatformerCameraManager)
			PlatformerCameraManager->ChangeState(new CameraStateAiming());

		if (Weapon)
		{
			Weapon->Hide(false);
			Weapon->Equip(GetMesh(), FName("RightHandSocket"));
		}
	}
}

void APlatformCharacter::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		ACannonProjectile* Projectile = Cast<ACannonProjectile>(OtherActor);
		if (Projectile != nullptr)
		{
			if (PlatformerCameraManager)
				PlatformerCameraManager->Shake(1.5f);

			Die();
		}

		APlatformObstacle* Obstacle = Cast<APlatformObstacle>(OtherActor);
		if (Obstacle != nullptr)
		{
			if (PlatformerCameraManager)
				PlatformerCameraManager->Shake(1.5f);
		}
	}
}

void APlatformCharacter::HandlePauseAction(const FInputActionValue& value)
{
	if (CharacterState != ECharacterState::ECS_IDLE) return;
	APlayerController* const PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		if (!HUD)
			GetHUD();

		HUD->PushMenuToHUD(HUDMenuID::HUD_PAUSE_M);
		PC->SetPause(true);
	}
}

void APlatformCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent = PlayerInputComponent;
	SetupInput();
}

void APlatformCharacter::SetupInput(const bool reset)
{
	if (!InputComponent) return;
	//we cast the UInputComponent to an UEnchancedInputComponent, and then bind each input action to its corresponding delegate 
	if (UEnhancedInputComponent* EnhancedInputComponent 
		= CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if(reset) EnhancedInputComponent->ClearActionBindings();
		//Move Input actions Binding
		//we use trigger events that occurred after one or more precessing ticks
		//We use ETriggerEvent::Started when we only want to get notified at the first frame of the trigger
		EnhancedInputComponent->BindAction(
			LookupInputAction, ETriggerEvent::Triggered, this, &APlatformCharacter::HandleLookAction);

		EnhancedInputComponent->BindAction(
			MoveInputAction, ETriggerEvent::Triggered, this, &APlatformCharacter::HandleMoveAction);

		EnhancedInputComponent->BindAction(
			JumpInputAction, ETriggerEvent::Started, this, &APlatformCharacter::HandleJumpAction);

		EnhancedInputComponent->BindAction(
			FireInputAction, ETriggerEvent::Started, this, &APlatformCharacter::HandleFireAction);

		EnhancedInputComponent->BindAction(
			ChangeLookInputAction, ETriggerEvent::Started, 
			this, &APlatformCharacter::HandleChangeCombatAction); 

		EnhancedInputComponent->BindAction(
			ChangeLookInputAction, ETriggerEvent::Completed, 
			this, &APlatformCharacter::HandleChangeCombatAction); 
		//HUD Input Actions Binding
		//We use ETriggerEvent::Started because we just need to get notify of the triggered the first frame of the triggered.
		//if we used ETriggerEvent::Triggered then the callback will be called multiple times and this could cause unexpected behaivors 
		EnhancedInputComponent->BindAction(
			SaveGameInputAction, ETriggerEvent::Started, this, &APlatformCharacter::HandleSaveAction);

		EnhancedInputComponent->BindAction(
			InventoryToggleInputAction, ETriggerEvent::Started, this, &APlatformCharacter::HandleInventoryAction);

		EnhancedInputComponent->BindAction(
			ShortCutsInputAction, ETriggerEvent::Started, this, &APlatformCharacter::HandleShortcutAction);

		EnhancedInputComponent->BindAction(
			PauseInputAction, ETriggerEvent::Started, this, &APlatformCharacter::HandlePauseAction);
	}

}

void APlatformCharacter::LevelUp()
{
	PlayerData.exp = 0;
	LevelUpEvent.Broadcast();
}

void APlatformCharacter::SetPlayerData(FPlayerDataStruct dataStruct)
{
	PlayerData = dataStruct;
	bCanJump = dataStruct.canJump;
	
	ConfigurateJumps();
	if (dataStruct.maxSpeed > 0)
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		if(Movement) Movement->MaxWalkSpeed = dataStruct.maxSpeed;
	}

	ExpChangedEvent.Broadcast(NextLevelExp, PlayerData.exp);
	LevelChangedEvent.Broadcast(PlayerData.level);
}

void APlatformCharacter::UpdateExp()
{
	PlayerData.exp++;
	if (PlayerData.exp < NextLevelExp)
		ExpChangedEvent.Broadcast(NextLevelExp, PlayerData.exp);
	else
	{
		ExpChangedEvent.Broadcast(NextLevelExp, PlayerData.exp);
		LevelUp();
	}
}

void APlatformCharacter::GetInputManagerSubsys()
{
	UWorld* world = GetWorld();
	if (world)
	{
		UGameInstance* gameInstance = world->GetGameInstance();
		if (gameInstance)
			InputManagerSubsys = gameInstance->GetSubsystem<UInputManagerSubsystem>();
	}
}

void APlatformCharacter::SpawnProjectile()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(World,0);
	//Get the middle of the screen location (where is the cross-head image)
	FVector StartLocation = CameraManager->K2_GetActorLocation();
	//Temporal impact location useful for the trace line
	FVector Offset(20, 20, 0);
	FVector ImpactLocation = (CameraManager->GetActorForwardVector() * 15000) + StartLocation;
	FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("SpawnSocket"),ERelativeTransformSpace::RTS_World);  

	FHitResult LineTraceResult;
	TArray<AActor*> toIgnoreActors;
	toIgnoreActors.Add(this);

	//Line Trace ignoring the Character
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, ImpactLocation, ETraceTypeQuery::TraceTypeQuery1, false, toIgnoreActors, EDrawDebugTrace::None, LineTraceResult, true))
	{
		//if we hit something on the lineTrace then the ImpactLocation is Updated
		if(LineTraceResult.bBlockingHit)
			ImpactLocation = LineTraceResult.ImpactPoint;
	}
	//In order to Spawn projectile on the hand socket of the character	
	FVector ProjectileSpawnLocation = SocketTransform.GetLocation();
	FVector SpawnPosition = ImpactLocation - ProjectileSpawnLocation;
	//Spawn rotation aiming from the Project Spawn Location to the ImpactLocation
	FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(SpawnPosition);
	FTransform SpawnTransform(SpawnRotation,ProjectileSpawnLocation,FVector(1.0f,1.0f,1.0f));

	//Configuration to spawn Projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	APlatformerProjectile* Projectile = World->SpawnActor<APlatformerProjectile>(ProjectileClass, SpawnTransform, SpawnParams);

	if (Projectile)
	{
		FVector LaunchDirection = SpawnRotation.Vector();
		//Fire projectile aiming to the target position
		Projectile->FireInDirection(LaunchDirection);
	}
}

void APlatformCharacter::Tick(float DeltaTime)
{
	if (GetCharacterMovement() && CharacterState == ECharacterState::ECS_JUMPING)
		CharacterState = !GetCharacterMovement()->IsFalling() ? ECharacterState::ECS_IDLE : CharacterState;
}

void APlatformCharacter::Destroyed()
{
	if (GameMode) GameMode->SaveDataBeforePlayerDestroy(PlayerData);
	if(bInInventory) ToggleInventory();
	AController* controllerRef = GetController();
	
	Super::Destroyed();
	if (GameMode && controllerRef)
	{
		EGameModeState gameModeState = GameMode->GetGameState();
		if(!GameMode->IsGameOver())
			GameMode->PlayerDestroyed(controllerRef);
	}

	Destroy();
}

