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
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
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


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World && WeaponClass)
	{
		EquippedWeapon = World->SpawnActor<AWeapon>(WeaponClass, GetActorLocation(), GetActorRotation());

		if (EquippedWeapon)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("LeftArmSocket"));
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->SetActorHiddenInGame(true);
		}
	}

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);
	
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) { return; }

	
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
	EnemyState = EEnemyState::EES_Chasing;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) { return; }

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(TargetRadius);
	EnemyController->MoveTo(MoveRequest);
}

void AEnemy::CheckDistanceToCombatTarget()
{
	if (EnemyState == EEnemyState::EES_Attacking) { return; }

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
	if (EnemyState == EEnemyState::EES_Attacking) { return; }
	
	if (AttackMontage)
	{
		EnemyState = EEnemyState::EES_Attacking;
		PlayMontage(AttackMontage, FName("Attack1"));
	}
}

bool AEnemy::InTargetRange(AActor* Target, const double& Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}
