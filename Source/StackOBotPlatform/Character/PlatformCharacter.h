#pragma once

#include "CoreMinimal.h"
#include "StackOBotPlatform/GameModeData.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "PlatformCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class APlatformGameMode;
class UInputMappingContext;
class APlatformHUD;
class UInputManagerSubsystem;
class AWeapon;
class APlatformerCameraManager;
struct FPlayerDataStruct;
struct FOrbData;

UCLASS()
class STACKOBOTPLATFORM_API APlatformCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlatformCharacter();

	/// <summary>
	/// Events to notify HUD
	/// </summary>
	DECLARE_EVENT_TwoParams(PlatformCharacter, FUpdateExpEvent,int32, int32)
	FUpdateExpEvent& OnExpChanged() { return ExpChangedEvent; }
	
	DECLARE_EVENT_OneParam(PlatformCharacter, FUpdateLevelEvent, int32)
	FUpdateLevelEvent& OnLevelChanged() { return LevelChangedEvent; }

	DECLARE_EVENT(PlatformCharacter, FLevelUpEvent)
	FLevelUpEvent& OnLevelUp(){ return LevelUpEvent; }

	/// <summary>
	/// Character Controller Actions
	/// </summary>
	/// <param name="value"></param>
	//Callback method when the move action is triggered
	UFUNCTION()
	void HandleMoveAction(const FInputActionValue& value);

	//Callback method for the Jump action when it is triggered 
	UFUNCTION()
	void HandleJumpAction(const FInputActionValue& value);

	//Callback method for the Look action when it is triggered
	UFUNCTION()
	void HandleLookAction(const FInputActionValue& value);
	//Callback method for the Fire action when it is triggered	
	UFUNCTION()
	void HandleFireAction(const FInputActionValue& value);
	//Callback method for the Camera action when it is triggered
	UFUNCTION()
	void HandleChangeCombatAction(const FInputActionValue& value);
	//Callback method for the Change combat action
	///////////////////////////////////////////////////////

	/// <summary>
	/// HUD Actions
	/// </summary>
	/// <param name="value"></param>
	//Callback method binded with HUD save action trigger
	UFUNCTION()
	void HandleSaveAction(const FInputActionValue& value);
	//Callback method binded with HUD Inventory action trigger
	UFUNCTION()
	void HandleInventoryAction(const FInputActionValue& value);
	//Callback method binded with the Shortcut action trigger
	UFUNCTION()
	void HandleShortcutAction(const FInputActionValue& value);

	//Callback method binded with the Shortcut action trigger
	UFUNCTION()
	void HandlePauseAction(const FInputActionValue& value);
	/////////////////////////////////////////////////////////
	//built-in ACharacter method to set up the input component for the character controller 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetCurrentDataTableRow(FLevelingInformationTable *data);
	void SetPlayerData(FPlayerDataStruct dataStruct);
	void UpdateExp();

	UFUNCTION(BlueprintCallable)
	void SetCanFire(bool CanFire) { bCanFire = CanFire; }

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();

	virtual void Tick(float deltaTime) override;
	FORCEINLINE FPlayerDataStruct GetPlayerData() { return PlayerData; };
	FORCEINLINE int32 GetExp() { return PlayerData.exp; };
	FORCEINLINE int32 GetLevel() { return PlayerData.level; };
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }
	FORCEINLINE void SetWeapon(AWeapon* weapon) { Weapon = weapon; }
	FORCEINLINE UCameraComponent* GetFollowCamera() { return FollowCamera; }
	FORCEINLINE USpringArmComponent* GetFollowBoom() { return BoomCamera; }
	FORCEINLINE UCameraComponent* GetDeathCamera() { return DeathCamera; }
	FORCEINLINE void SetCameraManager(APlatformerCameraManager* PlatformCameraManager) { PlatformerCameraManager = PlatformCameraManager; }
	FORCEINLINE bool IsUnderOrbEffect() { return bIsUnderOrbEffect; }

	UFUNCTION(BlueprintCallable)
	void Die();
	void ApplyOrbEffect(FOrbDataStruct OrbData);
	void DiscardOrbEffect(FOrbDataStruct OrbData);
	
protected:
	ECharacterState CharacterState;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//components
	//set set Uproperty as visible anywhere in order to can edit this properties from the blueprint viewport
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
	UCameraComponent* DeathCamera;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USpringArmComponent* BoomCamera;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class APlatformerProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category=Weapon)
	FVector CombatBoomCameraOffset;

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/// <summary>
	///  Character Controller Input Actions
	/// </summary>
	//This variable keep the reference to the IA_PlatformerMove Input action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input")
	UInputAction* MoveInputAction;
	//This variable keep the reference to the IA_PlatformerJump input action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input")
	UInputAction* JumpInputAction;
	//this variable keep the reference to the IA_PlatformerLook Input Action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input")
	UInputAction* LookupInputAction;
	//This is the variable to keep the reference to the IA_PlatformFire Input Action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* FireInputAction;
	//This is the variable to keep the reference to the IA_PlatformFire Input Action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* ChangeLookInputAction;

	/// <summary>
	/// HUD Controller Input Actions
	/// </summary>
	//This variable keep the reference to the IA_HUD_Save Input action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|UI Input Action")
	UInputAction* SaveGameInputAction;
	//This Variable Keep the reference to the IA_HUD_Inventory Input Action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|UI Input Action")
	UInputAction* InventoryToggleInputAction;
	//this variable Keep the reference to the IA_HUD_Shortcut Input action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|UI Input Action")
	UInputAction* ShortCutsInputAction;
	//this variable Keep the reference to the IA_HUD_Shortcut Input action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|UI Input Action")
	UInputAction* PauseInputAction;

	//This Variable Keep a reference to IMC_PlatformerContext Maping Action Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input|Mapping Context")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fall Data")
	float FallingTime = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire Settings")
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponSocket")
	FName WeaponSocketName;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFireAnimationEvent();

	virtual void Destroyed() override;
private:
	//Events variables to comunicate with UI and Update Exp and level
	FUpdateExpEvent ExpChangedEvent;
	FUpdateLevelEvent LevelChangedEvent;
	FLevelUpEvent LevelUpEvent;

	//Private variables
	APlatformGameMode* GameMode;
	FPlayerDataStruct PlayerData;

	APlatformHUD* HUD;
	UInputManagerSubsystem* InputManagerSubsys;
	int32 NextLevelExp;
	int32 MaxJumps = 1;
	int32 CurrentDataTableRow;
	//TODO: Refactor this to be used on PlatformerCameraManager.h
	FRotator BeforeCameraViewChangeRotation;

	//Other Components Variables
	AWeapon* Weapon;
	APlatformerCameraManager* PlatformerCameraManager;
	UInputComponent* InputComponent;

	bool bInInventory = false;
	bool bCanFire = true;
	bool bIsUnderOrbEffect = false;
	bool bCanJump = true;

	int jumpsCounter;

	//We use this method to set the UEnhancedInput map context to the 
	// Player controller
	void AttachEnhancedInput();
	void MoveForward(float value);
	void MoveRight(float value);
	void OnGameoverCallback();
	void BindNeededActors();
	void SetupCameraComponents();
	void LevelUp();
	void ConfigurateJumps();
	void GetHUD();
	void ToggleInventory();
	void GetInputManagerSubsys();
	void InitComponents();
	void InitWeapon();
	void SetupInput(const bool reset=false);
};
