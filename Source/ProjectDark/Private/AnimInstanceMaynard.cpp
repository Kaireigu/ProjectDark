// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceMaynard.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimInstanceMaynard::NativeInitializeAnimation()
{
	Enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (Enemy)
	{
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(Enemy->GetMovementComponent());
	}
}

void UAnimInstanceMaynard::NativeUpdateAnimation(float DeltaTime)
{

	if (CharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);
	}

	if (Enemy)
	{
		EnemyState = Enemy->GetEnemyState();
	}


}
