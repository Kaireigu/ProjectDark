// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Projectile.h"
#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Attributes.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique(FName("Hitable"));
	Tags.AddUnique(FName("Enemy"));
	Tags.AddUnique(FName("Lockable"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SightRadius = VisualRadius;
	PawnSensingComponent->SetPeripheralVisionAngle(PeripheralVisionAngle);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDistanceToCombatTarget();
	UpdatePatrolTarget();

}

void AEnemy::UpdatePatrolTarget()
{
	if (EnemyState != EEnemyState::EES_Patrolling) { return; }

	if (PatrolTarget && EnemyController)
	{
		if (InTargetRange(PatrolTarget, AcceptanceRadius))
		{
			TArray<AActor*> ValidTargets;

			for (AActor* Target : PatrolTargets)
			{
				if (Target != PatrolTarget)
				{
					ValidTargets.AddUnique(Target);
				}
			}

			const int32 NumOfPatrolTargets = ValidTargets.Num();

			if (NumOfPatrolTargets > 0)
			{
				const int32 TargetSelection = UKismetMathLibrary::RandomIntegerInRange(0, NumOfPatrolTargets - 1);

				AActor* Target = ValidTargets[TargetSelection];
				PatrolTarget = Target;
			}
		}

		MoveToTarget(PatrolTarget);
	}
}

void AEnemy::GetHit(AActor* OtherActor, const FVector& ImpactPoint)
{
	
	Super::GetHit(OtherActor, ImpactPoint);

}

float AEnemy::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		if (HealthBarComponent)
		{
			HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
			if (AttributeComponent->GetHealthPercent() <= 0.f)
			{
				Die();
			}
		}
	}

	return DamageAmount;
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World && WeaponClass)
	{
		EquippedWeapon = World->SpawnActor<AWeapon>(WeaponClass, GetActorLocation(), GetActorRotation());

		if (EquippedWeapon && HasRangedWeapon == false)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("LeftArmSocket"));
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->SetInstigator(this);
			EquippedWeapon->SetActorHiddenInGame(true);
			EquippedWeapon->SetItemStateEquipped();
			EnemyEquipState = ECharacterState::ECS_EquippedOneHanded;
		}
		else if (EquippedWeapon && HasRangedWeapon)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("LeftArmSocket"));
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->SetInstigator(this);
			EquippedWeapon->SetItemStateEquipped();
			EnemyEquipState = ECharacterState::ECS_EquippedLongbow;
		}
	}

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddUniqueDynamic(this, &AEnemy::PawnSeen);
	}

	EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);
	
}

void AEnemy::Die()
{
	if (EnemyState == EEnemyState::EES_Dead) { return; }

	Super::Die();
	EnemyState = EEnemyState::EES_Dead;
	EnemyDied.Broadcast(this);
	EquippedWeapon->Destroy();
}

void AEnemy::PlayHitReactMontage(const FVector& ImpactPoint)
{
	if (EnemyState != EEnemyState::EES_Dead)
	{
		Super::PlayHitReactMontage(ImpactPoint);
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing || EnemyState == EEnemyState::EES_Dead) { return; }

	
	if (SeenPawn->ActorHasTag("PlayerCharacter"))
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;

		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}

	}
}

void AEnemy::MontageEnd()
{
	if (EnemyState == EEnemyState::EES_Dead) { return; }

	EnemyState = EEnemyState::EES_Patrolling;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr || EnemyState == EEnemyState::EES_Dead || EnemyState == EEnemyState::EES_Attacking) { return; }

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);

	if (CombatTarget && HasRangedWeapon)
	{
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	}
	else
	{
		MoveRequest.SetAcceptanceRadius(TargetRadius);
	}
	EnemyController->MoveTo(MoveRequest);
}

void AEnemy::CheckDistanceToCombatTarget()
{
	if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Dead) { return; }

	if (CombatTarget)
	{
		if (!InTargetRange(CombatTarget, VisualRadius))
		{
			CombatTarget = nullptr;
			GetCharacterMovement()->MaxWalkSpeed = 150.f;
			EnemyState = EEnemyState::EES_Patrolling;
		}
		else if (InTargetRange(CombatTarget, AttackRadius))
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation()));
			Attack();
		}
		else if (InTargetRange(CombatTarget, VisualRadius))
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}

void AEnemy::Attack()
{
	if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Dead) { return; }
	
	if (AttackMontage && EnemyEquipState == ECharacterState::ECS_EquippedOneHanded)
	{
		EnemyState = EEnemyState::EES_Attacking;
		PlayMontage(AttackMontage, FName("Attack1"));
	}
	else if (ArcherAttackMontage && EnemyEquipState == ECharacterState::ECS_EquippedLongbow)
	{
		EnemyState = EEnemyState::EES_Attacking;

		if (CombatTarget)
		{
			CombatTargetLocationOnAttack = CombatTarget->GetActorLocation();
		}

		PlayMontage(ArcherAttackMontage, FName("DrawArrow"));
	}
}

void AEnemy::FireArrow()
{
	UWorld* World = GetWorld();

	if (World && ProjectileClass)
	{
		Arrow = World->SpawnActor<AProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation());

		if (Arrow && CombatTarget)
		{
			Arrow->FireProjectile(CombatTargetLocationOnAttack, GetActorLocation());
		}
		
	}
}

bool AEnemy::InTargetRange(AActor* Target, const double& Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}
