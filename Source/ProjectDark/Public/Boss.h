// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boss.generated.h"

class IInteractInterface;
class USoundBase;
class UAudioComponent;
/**
 * 
 */
UCLASS()
class PROJECTDARK_API ABoss : public AEnemy
{
	GENERATED_BODY()

public:
	ABoss();
	void Tick(float DeltaTime) override;
	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:

	void BeginPlay() override;
	void MoveToTarget(AActor* Target) override;
	void Die() override;
	void MontageEnd() override;

private:

	void EngageCombatTarget();
	void Attack() override;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* BossMusic;

	UAudioComponent* BossAudioComponent;

	bool bIsBossMusicPlaying = false;

	IInteractInterface* PlayerInteractInterface;

	float DegreeToRotateAfterTurning;
	
};
