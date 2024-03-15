// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDARK_API ABoss : public AEnemy
{
	GENERATED_BODY()

public:

	void Tick(float DeltaTime) override;

protected:

	void BeginPlay() override;
	void MoveToTarget(AActor* Target) override;

	void MontageEnd() override;

private:

	void EngageCombatTarget();
	void TurnToFaceCombatTarget();
	void Attack() override;

	float DegreeToRotateAfterTurning;
	
};
