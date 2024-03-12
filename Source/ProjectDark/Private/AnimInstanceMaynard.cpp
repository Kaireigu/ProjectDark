// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceMaynard.h"
#include "Enemy.h"

void UAnimInstanceMaynard::NativeInitializeAnimation()
{
	Enemy = Cast<AEnemy>(TryGetPawnOwner());
}

void UAnimInstanceMaynard::NativeUpdateAnimation(float DeltaTime)
{

	if (Enemy)
	{
		EnemyState = Enemy->GetEnemyState();
	}
}
