// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "BaseCharacter.h"
#include "InteractInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class AWeapon;
class UAnimMontage;
class UBoxComponent;
class AEnemy;
class UHUDOverlay;
class AShield;
class APotion;
class UPauseMenu;
class UNotificationScreen;

UCLASS()
class PROJECTDARK_API APlayerCharacter : public ABaseCharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetHit(AActor* OtherActor, const FVector& ImpactPoint) override;
	void GetHitWithDamage(const float& DamageAmount, const FVector& ImpactPoint) override;

	void InteractWithCheckpoint(const bool& BonfireLit, IInteractInterface* Bonfire) override;
	void SetCanGetOnLadder(const bool& CanGetOn, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder) override;
	void SetCanGetOffLadder(const bool& CanGetOff, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder) override;

	void SetHUDInteractText(const FString& InteractText) override;
	void ClearHUDInteractText() override;

	virtual void SetHUDNotifyText(const FString& NotifyText) override;
	virtual void ClearHUDNotifyText() override;

	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void PlayHitReactMontage(const FVector& ImpactPoint) override;
	void PlayLargeHitReactMontage() override;

	void SetupBossBar(const FString& BossName, const float& BossMaxHealth) override;
	void HideBossBar() override;
	void ShowBossBar() override;
	virtual void UpdateBossBar(const float& BossCurrentHealth) override;
	void SetDialogueText(const FString& TextToDisplay) override;
	void ClearDialogueText() override;
	void SetHitBossHeadLandPosition(const FVector& HitPosition) override;
	void SetCanOpenDoor(const bool& CanOpenDoor, IInteractInterface* Door) override;
	void StopInteractionWithCheckpoint() override;
	void DisplayVictoryAchieved() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetCanCombo(bool CanCombo);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void SetUnoccupied();

	UFUNCTION(BlueprintCallable)
	void SetWeaponSocketOnEquipping();

	UFUNCTION(BlueprintCallable)
	void SetShieldSocketOnEquipping();

	UFUNCTION(BlueprintCallable)
	void DrinkPotion();

	UFUNCTION(BlueprintCallable)
	void AttachPotionToHip();

	UFUNCTION(BlueprintCallable)
	void TurnClimbingOff();

	UFUNCTION(BlueprintCallable)
	void StopMovement();

	UFUNCTION(BlueprintCallable)
	void AddParryTag();

	UFUNCTION(BlueprintCallable)
	void RemoveParryTag();

	UFUNCTION(BlueprintCallable)
	void KickEnemy();

	void UseStamina(const float& StaminaAmount) override;
	void RechargeStamina() override;

	void PlayHitShieldSound() override;
	void Die() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleInstanceOnly)
	bool bCanInteractWithCheckpoint = false;

	UPROPERTY(VisibleInstanceOnly)
	bool bCanLightBonfire = false;

	UPROPERTY(VisibleInstanceOnly)
	bool bCanBackStabOrKickOrJumpAttack = false;

	UPROPERTY(EditAnywhere, Category = Movement)
	float OrientRotationRateYaw = 1080.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float Walkspeed= 200.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float TimeBufferToBackStab = 0.25f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float BackStabAttackRange = 200.f;

	UPROPERTY(EditAnywhere, Category = "Stamina Cost")
	float LightAttackStaminaCost = 20.f;

	UPROPERTY(EditAnywhere, Category = "Stamina Cost")
	float HeavyAttackStaminaCost = 40.f;

	UPROPERTY(EditAnywhere, Category = "Stamina Cost")
	float BackstepStaminaCost = 20.f;

	UPROPERTY(EditAnywhere, Category = "Stamina Cost")
	float RollStaminaCost = 30.f;

	UPROPERTY(EditAnywhere, Category = "Stamina Cost")
	float BlockingStaminaCost = 30.f;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EPressedAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* RollOrBackStepAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LockOnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SwitchLockedTargetAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* StopBlockAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* UseItemAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SelectRightHandAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SelectLeftHandAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* StopSprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* TapR1Action;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* TapR2Action;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* PressedL2Action;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DPadUpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DPadDownAction;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontageOneHanded;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* BackStepMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* BlockMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* PotionMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SitMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ClimbMontage;

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* LockOnBox;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* StartTraceLocation;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* EndTraceLocation;

	UPROPERTY(EditAnywhere, Category = Components)
	TSubclassOf<UPauseMenu> PauseMenuWidgetBP;

	UPROPERTY(EditAnywhere, Category = Components)
	TSubclassOf<UNotificationScreen> NotificationScreenWidgetBP;

	UPROPERTY(EditAnywhere, Category = Components)
	TSubclassOf<UUserWidget> CreditsBP;

	UPROPERTY()
	UPauseMenu* PauseMenuWidget;

	UPROPERTY()
	UNotificationScreen* NotificationScreenWidget;

	UPROPERTY()
	UUserWidget* CreditsWidget;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraHeightLockedOn = 80.f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraBoomTargetLength = 300.f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float BackStabCameraBoomTargetLength = 200.f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float LockOffDistance = 2000.f;

	UPROPERTY(VisibleInstanceOnly)
	AShield* EquippedShield;

	UPROPERTY(VisibleInstanceOnly)
	APotion* EquippedPotion;

	UPROPERTY(VisibleInstanceOnly)
	FTransform SpawnLocation;

	UPROPERTY(EditAnywhere, Category = Movement)
	float LookSensitvity = 0.75f;

private:

	void SetDefaultControllerValues();
	void InitialiseComponents();
	void InitialiseSubsystem();

	void BindInputActions(UInputComponent* PlayerInputComponent);

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void EKeyPressed(const FInputActionValue& value);
	void Attack(const FInputActionValue& value);
	void HeavyAttack(const FInputActionValue& value);
	void RollOrBackStep(const FInputActionValue& value);
	void LockOn(const FInputActionValue& value);
	void SwitchLockOnTarget(const FInputActionValue& value);
	void Block(const FInputActionValue& value);
	void StopBlock(const FInputActionValue& value);
	void UseItem(const FInputActionValue& value);
	void SwitchRightHand(const FInputActionValue& value);
	void SwitchLeftHand(const FInputActionValue& value);
	void Sprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);
	void TapR1(const FInputActionValue& value);
	void TapR2(const FInputActionValue& value);
	void PressedL2(const FInputActionValue& value);
	void PauseButtonPressed(const FInputActionValue& value);
	void SelectMenuUp(const FInputActionValue& value);
	void SelectMenuDown(const FInputActionValue& value);


	UFUNCTION()
	void OnLockBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLockBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void LockOnBoxTrace();
	void UpdateLockOnTarget(float& DeltaTime);
	void LookAtCurrentTarget(float& DeltaTime);
	void DetermineLeftAndRightTargets();
	void DetermineFirstLockOnTarget();
	void CheckLockOnTargetDistance();
	void SetLockOffValues();
	void ClearNotificationScreen();

	UFUNCTION()
	void OnEnemyDeath(AActor* Enemy);

	bool IsOccupied();
	bool IsUnoccupied();
	bool CanEquip();
	bool CanUnequip();
	bool IsMoving();
	bool IsNotMoving();
	bool IsEquippedWithOneHandedWeapon();
	bool IsUnequipped();
	bool IsEquippedSwordAndShield();
	bool IsShieldEquipped();
	bool IsNotHitReacting();

	bool bCanCombo = false;
	bool bComboActive = false;
	bool bIsLockingOn = false;
	bool bIsFirstTimeLockingOn = true;
	bool bCanGetOnLadder = false;
	bool bCanGetOffLadder = false;
	bool bShouldDoIKTrace = true;
	bool bCanUseDodgeBackAttack = false;
	bool bCanShowNotifyText = false;
	bool bCanOpenDoor = false;
	bool bSwitchToSecondaryWeapon = false;

	FVector LadderPosition;
	FVector LadderStartPosition;
	FRotator LadderFacingRotation;
	IInteractInterface* LadderInUse = nullptr;

	void ReceiveHealth(const float& HealAmount);
	void ReceiveDamage(const float& DamageAmount);
	void StartStaminaRecharge();
	void StopStaminaRecharge();
	void StartDrainStamina();
	void StopDrainStamina();
	float GetStamina();

	void AddActorTags();
	void SetupHUD();
	void UpdateHUD(const float& DeltaTime);
	void BindDelegateFunctions();
	void PickUpWeapon();
	void PickUpShield();
	void PickUpPotion();
	void CheckCanSitAtCheckpoint();
	void TurnClimbingOn();
	void SetCannotBackstabOrKickOrJumpAttack();
	void Respawn();

	FTimerHandle CannotBackstabTimerHandle;
	FTimerHandle ClearNotifyTimerHandle;
	FTimerHandle NotifiScreenDisplayHandle;

	UPROPERTY(VisibleInstanceOnly)
	TArray<AActor*> LockableEnemies;

	UPROPERTY()
	AActor* CurrentEnemyTarget;

	UPROPERTY()
	AActor* EnemyTargetRight;

	UPROPERTY()
	AActor* EnemyTargetLeft;

	IHitInterface* CurrentEnemyTargetHitInterface;

	IInteractInterface* CurrentDoor;

	IInteractInterface* CurrentBonfire;

	UPROPERTY()
	AEnemy* IsDeadEnemy;

	FVector LockOnTargetPosition = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedSecondaryWeapon;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	UHUDOverlay* HUDOverlay;

	float CurrentBossMaxHealth;
	double ToEnemyAngle;


public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }
	FORCEINLINE EActionState GetActionState() { return ActionState; }
	FORCEINLINE bool GetShouldDoIKTrace() { return bShouldDoIKTrace; }
	FORCEINLINE bool GetIsLockedOn() { return CurrentEnemyTarget != nullptr; }
	double GetMovementX();
	double GetMovementY();
};
