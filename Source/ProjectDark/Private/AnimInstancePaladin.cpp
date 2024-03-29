// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstancePaladin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"

void UAnimInstancePaladin::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (PlayerCharacter)
	{
		PlayerMovementComponent = Cast<UCharacterMovementComponent>(PlayerCharacter->GetMovementComponent());
	}
}

void UAnimInstancePaladin::NativeUpdateAnimation(float DeltaTime)
{
	if (PlayerMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovementComponent->Velocity);
		IsFalling = PlayerMovementComponent->IsFalling();
	}

	if (PlayerCharacter)
	{
		CharacterState = PlayerCharacter->GetCharacterState();
		ActionState = PlayerCharacter->GetActionState();
		bShouldDoIKTrace = PlayerCharacter->GetShouldDoIKTrace();
		IsLockedOn = PlayerCharacter->GetIsLockedOn();
		MovementDirectionX = FMath::Clamp(PlayerCharacter->GetMovementX(), -1.f, 1.f);
		MovementDirectionY = FMath::Clamp(PlayerCharacter->GetMovementY(), -1.f, 1.f);
	}
}
