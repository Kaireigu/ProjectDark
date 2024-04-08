// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Weapon.h"
#include "Attributes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributes>(TEXT("Attribute Component"));

	BackStabLocation = CreateDefaultSubobject<USceneComponent>(TEXT("BackStab Location"));
	BackStabLocation->SetupAttachment(GetRootComponent());

	FrontStabLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FrontStab Location"));
	FrontStabLocation->SetupAttachment(GetRootComponent());
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
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorLocation());
	}
}

void ABaseCharacter::GetHitWithDamage(const float& DamageAmount, const FVector& ImpactPoint)
{
	PlayHitReactMontage(ImpactPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
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

void ABaseCharacter::PlayStaggerMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		PlayMontage(HitReactMontage, FName("Stagger"));
		Tags.AddUnique(FName("Staggered"));

		if (StaggeredSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, StaggeredSound, GetActorLocation());
		}
	}
}

void ABaseCharacter::PlayBackStab()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Stop(0.f, HitReactMontage);
		PlayMontage(HitReactMontage, FName("BackStab"));
		Tags.Remove(FName("Staggered"));
		AddBackStabTag();
	}
}

FVector ABaseCharacter::GetBackStabLocation()
{
	if (BackStabLocation)
	{
		return BackStabLocation->GetComponentLocation();
	}
	else
	{
		return FVector::ZeroVector;
	}
}

FVector ABaseCharacter::GetFrontStabLocation()
{
	if (FrontStabLocation)
	{
		return FrontStabLocation->GetComponentLocation();
	}
	else
	{
		return FVector::ZeroVector;
	}
}

bool ABaseCharacter::IsAnyMontagePlaying()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		return AnimInstance->IsAnyMontagePlaying();
	}

	return false;
}

bool ABaseCharacter::IsBehind(const AActor* OtherActor)
{
	double Theta = GetTheta(GetActorForwardVector(), OtherActor->GetActorLocation());

	if (Theta > 0.f && Theta >= 90.f)
	{
		return true;
	}
	else if (Theta < 0.f && Theta <= -90.f)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool ABaseCharacter::IsFacing(const AActor* OtherActor)
{
	double Theta = GetTheta(GetActorForwardVector(), OtherActor->GetActorLocation());

	if (Theta > 0.f && Theta <= 90.f)
	{
		return true;
	}
	else if (Theta < 0.f && Theta >= -90.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ABaseCharacter::EnemyIsFacingMe(const AActor* OtherActor)
{
	double Theta = GetThetaFromActors(OtherActor, this);

	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, FString::SanitizeFloat(Theta));

	if (Theta > 0.f && Theta <= 135.f)
	{
		return true;
	}
	else if (Theta < 0.f && Theta >= -135.f)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool ABaseCharacter::IsNotInFront(const AActor* OtherActor)
{
	double Theta = GetTheta(GetActorForwardVector(), OtherActor->GetActorLocation());

	if (Theta <= 45.f && Theta > -45.f)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ABaseCharacter::AddBackStabTag()
{
	Tags.AddUnique(FName("BackStabActive"));
}

void ABaseCharacter::RemoveBackStabTag()
{
	Tags.Remove(FName("BackStabActive"));
}

void ABaseCharacter::PlayHitShieldSound()
{
	if (ShieldHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShieldHitSound, GetActorLocation());
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

double ABaseCharacter::GetThetaFromActors(const AActor* ForwardVectorActor, const AActor* LocationActor)
{
	const FVector EnemyLocation = LocationActor->GetActorLocation();
	const FVector EnemyLocationLowered = FVector(EnemyLocation.X, EnemyLocation.Y, ForwardVectorActor->GetActorLocation().Z);
	const FVector ForwardVector = ForwardVectorActor->GetActorForwardVector();
	const FVector ToEnemy = (EnemyLocationLowered - ForwardVectorActor->GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(ForwardVector, ToEnemy);
	double Theta = UKismetMathLibrary::Acos(CosTheta);
	Theta = UKismetMathLibrary::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToEnemy);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

void ABaseCharacter::Die()
{
	PlayMontage(DeathMontage, FName("Death1"));

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

bool ABaseCharacter::InTargetRange(AActor* Target, const double& Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

bool ABaseCharacter::InTargetRange(const FVector& Target, const double& Radius)
{
	const double DistanceToTarget = (Target - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void ABaseCharacter::EnableWeaponCollision()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollision(true);
		EquippedWeapon->IgnoredActors.Empty();
	}
}

void ABaseCharacter::DisableWeaponCollision()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollision(false);
	}
}

void ABaseCharacter::SetDeathValues()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.f);
}

