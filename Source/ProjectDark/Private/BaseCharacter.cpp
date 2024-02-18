// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::GetHit(AActor* OtherActor, const FVector& ImpactPoint)
{
	PlayHitReactMontage(ImpactPoint);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::PlayMontage(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ABaseCharacter::PlayHitReactMontage(const FVector& ImpactPoint)
{
	double Theta = GetTheta(GetActorForwardVector(), ImpactPoint);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{

		if (Theta <= 45.f && Theta > -45.f)
		{
			PlayMontage(HitReactMontage, FName("ReactFromFront"));
		}
		else if (Theta >= 45.f && Theta < 135.f)
		{
			PlayMontage(HitReactMontage, FName("ReactFromRight"));
		}
		else if (Theta >= 135.f || Theta <= -135.f)
		{
			PlayMontage(HitReactMontage, FName("ReactFromBack"));
		}
		else if (Theta >= -135.f && Theta < -45.f)
		{
			PlayMontage(HitReactMontage, FName("ReactFromLeft"));
		}

	}
}

double ABaseCharacter::GetTheta(const FVector& Forward, const FVector& OtherActorLocation)
{
	const FVector EnemyLocation = OtherActorLocation;
	const FVector EnemyLocationLowered = FVector(EnemyLocation.X, EnemyLocation.Y, GetActorLocation().Z);
	const FVector ForwardVector = Forward;
	const FVector ToEnemy = (EnemyLocationLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToEnemy);
	double Theta = UKismetMathLibrary::Acos(CosTheta);
	Theta = UKismetMathLibrary::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToEnemy);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

void ABaseCharacter::EnableWeaponCollision()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollision(true);
	}
}

void ABaseCharacter::DisableWeaponCollision()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollision(false);
	}
}

