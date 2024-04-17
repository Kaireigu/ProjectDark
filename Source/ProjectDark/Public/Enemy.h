// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, AActor*, Enemy);

class UAnimMontage;
class AAIController;
class UPawnSensingComponent;
class AWeapon;
class AShield;
class AItem;
class AProjectile;
class UHealthBarComponent;
class UMotionWarpingComponent;

UCLASS()
class PROJECTDARK_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	void UpdatePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void GetHit(AActor* OtherActor, const FVector& ImpactPoint) override;
	void BeLockedOnTo() override;
	void BeLockedOff() override;
	void InterfacePlayHitReact(const FVector& ImpactPoint) override;
	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FOnEnemyDeath EnemyDied;

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void PlayHitReactMontage(const FVector& ImpactPoint) override;
	FVector GetTranslationWarpTarget();
	FRotator GetRotationWarpTarget();
	void CheckDistanceToCombatTarget();
	virtual void MoveToTarget(AActor* Target);
	void MoveToTargetLocation(const FVector& Target);
	virtual void Attack();
	void Block();
	void Strafe();
	void StartAttackTimer();
	void ClearAttackTimer();

	void AddInitialTags();

	UFUNCTION(BlueprintCallable)
	virtual void MontageEnd();

	UFUNCTION(BlueprintCallable)
	void FireArrow();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	double WarpTargetDistance = 75.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float WalkSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float ChaseSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float DistanceToStrafe = 200.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AShield> ShieldClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AActor> TargetPoint;

	UPROPERTY()
	AShield* EquippedShield = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool HasRangedWeapon = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool HasMeleeWeapon = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool HasShield = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AProjectile* Arrow;

	UPROPERTY(EditAnywhere, Category = "Loot Item")
	TSubclassOf<AItem> LootItem;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	ECharacterState EnemyEquipState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly)
	AActor* CombatTarget;

	UPROPERTY(VisibleInstanceOnly)
	FVector LastKnownLocationOfCombatTarget;

	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY()
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ArcherAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SwordAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* BlockMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ThrustAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* StrafeMontage;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double TargetRadius = 15.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double CannotStrafeRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AcceptanceRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double VisualRadius = 2000.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PeripheralVisionAngle = 45.f;

	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarComponent;

	UPROPERTY(VisibleInstanceOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(EditAnywhere, Category = Properties)
	bool IsEnemyHarmless = false;

	FVector SpawnPosition;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float AttackCooldown = 0.5f;

	UPROPERTY(VisibleAnywhere)
	bool bAttackCooldownFinished = true;

	FTimerHandle AttackCooldownTimerHandle;

private:

	void InitialiseComponents();

public:	
	FORCEINLINE const EEnemyState& GetEnemyState() { return EnemyState; }

};
