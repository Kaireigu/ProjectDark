// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class AAIController;
class UPawnSensingComponent;
class AWeapon;

UCLASS()
class PROJECTDARK_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	void UpdatePatrolTarget();

	void GetHit(AActor* OtherActor, const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UFUNCTION(BlueprintCallable)
	void MontageEnd();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(VisibleInstanceOnly)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double TargetRadius = 15.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AcceptanceRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double VisualRadius = 2000.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PeripheralVisionAngle = 45.f;

	UPROPERTY()
	AAIController* EnemyController;


private:

	void MoveToTarget(AActor* Target);
	void CheckDistanceToCombatTarget();
	void Attack();

	bool InTargetRange(AActor* Target, const double& Radius);

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

public:	


};
