// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "AnimInstanceMaynard.generated.h"

class UCharacterMovementComponent;

class AEnemy;
/**
 * 
 */
UCLASS()
class PROJECTDARK_API UAnimInstanceMaynard : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	AEnemy* Enemy;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;
	
private:

	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaTime) override;

	UCharacterMovementComponent* CharacterMovementComponent;
};
