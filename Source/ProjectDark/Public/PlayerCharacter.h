// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "BaseCharacter.h"
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

UCLASS()
class PROJECTDARK_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetHit(AActor* OtherActor, const FVector& ImpactPoint) override;

	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditAnywhere, Category = Movement)
	float OrientRotationRateYaw = 1080.f;

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

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* LockOnBox;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* StartTraceLocation;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* EndTraceLocation;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraHeightLockedOn = 50.f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float LockOffDistance = 2000.f;

	UPROPERTY(VisibleInstanceOnly)
	AShield* EquippedShield;

	UPROPERTY(VisibleInstanceOnly)
	APotion* EquippedPotion;

private:

	void SetDefaultControllerValues();
	void InitialiseComponents();
	void InitialiseSubsystem();

	void BindInputActions(UInputComponent* PlayerInputComponent);

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void EKeyPressed(const FInputActionValue& value);
	void Attack(const FInputActionValue& value);
	void RollOrBackStep(const FInputActionValue& value);
	void LockOn(const FInputActionValue& value);
	void SwitchLockOnTarget(const FInputActionValue& value);
	void Block(const FInputActionValue& value);
	void StopBlock(const FInputActionValue& value);
	void UseItem(const FInputActionValue& value);
	void SwitchRightHand(const FInputActionValue& value);
	void SwitchLeftHand(const FInputActionValue& value);

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

	bool bCanCombo = false;
	bool bComboActive = false;
	bool bIsLockingOn = false;
	bool bIsFirstTimeLockingOn = true;

	UPROPERTY(VisibleInstanceOnly)
	TArray<AActor*> LockableEnemies;

	UPROPERTY()
	AActor* CurrentEnemyTarget;

	UPROPERTY()
	AActor* EnemyTargetRight;

	UPROPERTY()
	AActor* EnemyTargetLeft;

	UPROPERTY()
	AEnemy* IsDeadEnemy;

	FVector LockOnTargetPosition = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	UHUDOverlay* HUDOverlay;

public:	
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }
	FORCEINLINE EActionState GetActionState() { return ActionState; }
};
