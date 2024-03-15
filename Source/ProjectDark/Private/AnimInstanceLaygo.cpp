// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceLaygo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Boss.h"

void UAnimInstanceLaygo::NativeInitializeAnimation()
{
	Boss = Cast<ABoss>(TryGetPawnOwner());
}

void UAnimInstanceLaygo::NativeUpdateAnimation(float DeltaTime)
{
	if (Boss)
	{
		EnemyState = Boss->GetEnemyState();
		GroundSpeed = UKismetMathLibrary::VSizeXY(Boss->GetMovementComponent()->Velocity);
	}
}
