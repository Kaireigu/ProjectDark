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
#include "Components/AudioComponent.h"


ABoss::ABoss()
{
	Tags.AddUnique(FName("Boss"));
}

void ABoss::Tick(float DeltaTime)
{
	EngageCombatTarget();

	if (CombatTarget && MotionWarpingComponent)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("CombatTarget"), GetTranslationWarpTarget());

		if (BossMusic && bIsBossMusicPlaying == false)
		{
			BossAudioComponent = UGameplayStatics::SpawnSoundAttached(BossMusic, GetRootComponent());
			bIsBossMusicPlaying = true;
		}
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

	if (BossAudioComponent)
	{
		BossAudioComponent->Stop();
	}

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
	if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Dead || AttackMontage == nullptr) { return; }

	EnemyState = EEnemyState::EES_Attacking;
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
