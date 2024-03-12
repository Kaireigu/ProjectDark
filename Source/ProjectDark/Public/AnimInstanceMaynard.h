// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "AnimInstanceMaynard.generated.h"


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
	
private:

	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaTime) override;
};
