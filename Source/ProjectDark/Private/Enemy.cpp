// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique(FName("Hitable"));

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{

	const FVector Forward = GetActorForwardVector(); // Gets the Normalized vector
	const FVector ImpactPointLowered = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHitNormalized = (ImpactPointLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHitNormalized);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHitNormalized);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::SanitizeFloat(Theta));
		}

		if (Theta <= 45.f && Theta > -45.f)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(FName("ReactFromFront"), HitReactMontage);
		}
		else if (Theta >= 45.f && Theta < 135.f)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(FName("ReactFromRight"), HitReactMontage);
		}
		else if (Theta >= 135.f || Theta <= -135.f)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(FName("ReactFromBack"), HitReactMontage);
		}
		else if (Theta >= -135.f && Theta < -45.f)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(FName("ReactFromLeft"), HitReactMontage);
		}

	}
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}
