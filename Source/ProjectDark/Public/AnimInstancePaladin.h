// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstancePaladin.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PROJECTDARK_API UAnimInstancePaladin : public UAnimInstance
{
	GENERATED_BODY()

public:

protected:

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;

private:

	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaTime) override;

	UCharacterMovementComponent* PlayerMovementComponent;
	
};
