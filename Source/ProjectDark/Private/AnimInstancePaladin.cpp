// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstancePaladin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"

void UAnimInstancePaladin::NativeInitializeAnimation()
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (PlayerChar)
	{
		PlayerMovementComponent = Cast<UCharacterMovementComponent>(PlayerChar->GetMovementComponent());
	}
}

void UAnimInstancePaladin::NativeUpdateAnimation(float DeltaTime)
{
	if (PlayerMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovementComponent->Velocity);
		IsFalling = PlayerMovementComponent->IsFalling();
	}
}
