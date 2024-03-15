// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "AnimInstanceLaygo.generated.h"

class ABoss;
/**
 * 
 */
UCLASS()
class PROJECTDARK_API UAnimInstanceLaygo : public UAnimInstance
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly)
	ABoss* Boss;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

private:

	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaTime) override;
};
