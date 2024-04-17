// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Perception/PawnSensingComponent.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "InteractInterface.h"
#include "Attributes.h"
#include "Kismet/GameplayStatics.h"


ABoss::ABoss()
{
	Tags.AddUnique(FName("Boss"));

	HitInHeadPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Hit In Head Position"));
	HitInHeadPosition->SetupAttachment(GetRootComponent());
}

void ABoss::Tick(float DeltaTime)
{
	EngageCombatTarget();

	if (CombatTarget && MotionWarpingComponent)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("CombatTarget"), GetTranslationWarpTarget());

	}

	if (GetActorLocation().Z < SpawnPosition.Z - 1000.f)
	{
		Die();
	}
}

float ABoss::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (PlayerInteractInterface && AttributeComponent)
	{
		PlayerInteractInterface->UpdateBossBar(AttributeComponent->GetHealthPercent());
	}

	return 0.0f;
}

void ABoss::PlayHeadHitMontage()
{
	PlayMontage(HeadHitMontage, FName("HeadHit"));
	UGameplayStatics::ApplyDamage(this, 120.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	EnemyState = EEnemyState::EES_Strafing;

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorLocation());
	}

	if (PlayerInteractInterface == nullptr && CombatTarget)
	{
		PlayerInteractInterface = Cast<IInteractInterface>(CombatTarget);

		if (PlayerInteractInterface && AttributeComponent)
		{
			PlayerInteractInterface->SetupBossBar(FString("Asylum Demon Leygo"), AttributeComponent->GetMaxHealth());
		}
	}

	if (PlayerInteractInterface && AttributeComponent)
	{
		PlayerInteractInterface->UpdateBossBar(AttributeComponent->GetHealthPercent());
	}
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

}

void ABoss::MoveToTarget(AActor* Target)
{
	
	if (EnemyController == nullptr || Target == nullptr || EnemyState == EEnemyState::EES_Dead || EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Blocking || EnemyState == EEnemyState::EES_Strafing) { return; }

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	EnemyController->MoveTo(MoveRequest);

	if (PlayerInteractInterface == nullptr)
	{
		PlayerInteractInterface = Cast<IInteractInterface>(Target);

		if (PlayerInteractInterface && AttributeComponent)
		{
			PlayerInteractInterface->SetupBossBar(FString("Asylum Demon Leygo"), AttributeComponent->GetMaxHealth());
		}
	}

}

void ABoss::Die()
{
	Super::Die();

	if (PlayerInteractInterface)
	{
		PlayerInteractInterface->HideBossBar();
	}
}

void ABoss::MontageEnd()
{
	if (EnemyState == EEnemyState::EES_Dead) { return; }


	EnemyState = EEnemyState::EES_Chasing;
}

void ABoss::EngageCombatTarget()
{
	if (CombatTarget && InTargetRange(CombatTarget, AttackRadius))
	{
		LastKnownLocationOfCombatTarget = CombatTarget->GetActorLocation();
	}

	if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Blocking || EnemyState == EEnemyState::EES_Strafing || EnemyState == EEnemyState::EES_Dead) { return; }


	if (CombatTarget)
	{

		if (InTargetRange(CombatTarget, AttackRadius))
		{
			Attack();
		}
		else
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}

void ABoss::Attack()
{
	if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Dead || AttackMontage == nullptr || bAttackCooldownFinished == false) { return; }

	EnemyState = EEnemyState::EES_Attacking;
	StartAttackTimer();
	int32 Selection = FMath::RandRange(1, 3);

	switch (Selection)
	{
	case 0:
		PlayMontage(AttackMontage, FName("GroundPound"));
		break;

	case 1:
		PlayMontage(AttackMontage, FName("JumpAttack"));
		break;

	case 2:
		PlayMontage(AttackMontage, FName("LowSwing"));
		break;

	case 3:
		PlayMontage(AttackMontage, FName("HeavySwing"));
		break;

	default:
		break;
	}

}

FVector ABoss::GetHitInTheHeadPosition()
{
	if (HitInHeadPosition == nullptr) { return FVector::ZeroVector; }

	return HitInHeadPosition->GetComponentLocation();
}
