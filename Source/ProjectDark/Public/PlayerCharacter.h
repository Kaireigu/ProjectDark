// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class AWeapon;
class UAnimMontage;
class UBoxComponent;

UCLASS()
class PROJECTDARK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void DisableWeaponCollision();

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

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontageOneHanded;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* BackStepMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* LockOnBox;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* StartTraceLocation;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* EndTraceLocation;

private:

	void SetDefaultControllerValues();
	void InitialiseComponents();
	void InitialiseSubsystem();

	void BindInputActions(UInputComponent* PlayerInputComponent);
	void PlayMontage(UAnimMontage* Montage, const FName& SectionName);

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void EKeyPressed(const FInputActionValue& value);
	void Attack(const FInputActionValue& value);
	void RollOrBackStep(const FInputActionValue& value);
	void LockOn(const FInputActionValue& value);

	void LockOnBoxTrace();
	void UpdateLockOnTarget(float& DeltaTime);

	bool IsOccupied();
	bool IsUnoccupied();
	bool CanEquip();
	bool CanUnequip();
	bool IsMoving();
	bool IsNotMoving();
	bool IsEquippedWithOneHandedWeapon();
	bool IsUnequipped();

	bool bCanCombo = false;
	bool bComboActive = false;
	bool bIsLockingOn = false;

	UPROPERTY(VisibleInstanceOnly)
	TArray<AActor*> LockableEnemies;

	UPROPERTY()
	AActor* EnemyTarget;

	FVector LockOnTargetPosition = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraHeightLockedOn = 75.f;

public:	
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }
};
