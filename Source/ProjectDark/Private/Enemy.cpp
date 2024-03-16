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
#include "Shield.h"
#include "Item.h"
#include "Projectile.h"
#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Attributes.h"
#include "MotionWarpingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	AddInitialTags();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	InitialiseComponents();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDistanceToCombatTarget();
	UpdatePatrolTarget();

	if (CombatTarget && MotionWarpingComponent)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("CombatTarget"), GetTranslationWarpTarget());
	}

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

void AEnemy::BeLockedOnTo()
{
	HealthBarComponent->SetVisibility(true);
}

void AEnemy::BeLockedOff()
{
	HealthBarComponent->SetVisibility(false);
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
		EquippedShield = World->SpawnActor<AShield>(ShieldClass, GetActorLocation(), GetActorRotation());

		if (EquippedWeapon && HasRangedWeapon == false && HasMeleeWeapon == false)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("LeftArmSocket"));
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->SetInstigator(this);
			EquippedWeapon->SetActorHiddenInGame(true);
			EquippedWeapon->SetItemStateEquipped();
			EnemyEquipState = ECharacterState::ECS_Unequipped;
		}
		else if (EquippedWeapon && HasRangedWeapon)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("LeftArmSocket"));
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->SetInstigator(this);
			EquippedWeapon->SetItemStateEquipped();
			EnemyEquipState = ECharacterState::ECS_EquippedLongbow;
		}
		else if (EquippedWeapon && HasMeleeWeapon)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->SetInstigator(this);
			EquippedWeapon->SetItemStateEquipped();
			EnemyEquipState = ECharacterState::ECS_EquippedOneHanded;
		}
		
		if (EquippedShield && HasShield)
		{
			EquippedShield->AttachMeshToSocket(GetMesh(), FName("LeftForeArmSocket"));
			EquippedShield->SetOwner(this);
			EquippedShield->SetInstigator(this);
			EquippedShield->SetItemStateEquipped();
			EnemyEquipState = ECharacterState::ECS_EquippedSwordAndShield;
		}
	}

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddUniqueDynamic(this, &AEnemy::PawnSeen);
	}

	EnemyController = Cast<AAIController>(GetController());
	
}

void AEnemy::Die()
{
	if (EnemyState == EEnemyState::EES_Dead) { return; }

	Super::Die();
	EnemyState = EEnemyState::EES_Dead;
	EnemyDied.Broadcast(this);

	if (EquippedWeapon) { EquippedWeapon->Destroy(); }
	if (EquippedShield) { EquippedShield->Destroy(); }
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
	if (EnemyState != EEnemyState::EES_Patrolling) { return; }

	
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
	Tags.Remove(FName("Blocking"));
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr || EnemyState == EEnemyState::EES_Dead || EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Blocking || EnemyState == EEnemyState::EES_Strafing) { return; }

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);

	if (CombatTarget)
	{
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	}
	else
	{
		MoveRequest.SetAcceptanceRadius(TargetRadius);
	}
	EnemyController->MoveTo(MoveRequest);
}

void AEnemy::MoveToTargetLocation(const FVector& Target)
{
	if (EnemyController == nullptr || EnemyState == EEnemyState::EES_Dead || EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Blocking) { return; }

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Target);

	MoveRequest.SetAcceptanceRadius(TargetRadius);
	
	EnemyController->MoveTo(MoveRequest);
}

void AEnemy::CheckDistanceToCombatTarget()
{
	if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Blocking || EnemyState == EEnemyState::EES_Strafing || EnemyState == EEnemyState::EES_Dead) { return; }

	if (CombatTarget)
	{
		if (!InTargetRange(CombatTarget, VisualRadius))
		{
			CombatTarget = nullptr;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			EnemyState = EEnemyState::EES_Patrolling;
		}
		else if (InTargetRange(CombatTarget, AttackRadius))
		{
			int32 Selection = FMath::RandRange(0, 3);

			if (HasShield)
			{

				switch (Selection)
				{
				case 0:
					Attack();
					break;

				case 1:
					Attack();
					break;

				case 2:
					Block();
					break;

				case 3:
					Strafe();
					break;

				default:
					break;
				}
			}
			else if (HasRangedWeapon == false)
			{
				switch (Selection)
				{
				case 0:
					Attack();
					break;

				case 1:
					Attack();
					break;

				case 2:
					Attack();
					break;

				case 3:
					Strafe();
					break;

				default:
					break;
				}
			}
			else
			{
				Attack();
			}


		}
		else if (InTargetRange(CombatTarget, VisualRadius) && InTargetRange(CombatTarget, AttackRadius) == false)
		{
			GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
			EnemyState = EEnemyState::EES_Chasing;
			Tags.Remove(FName("Blocking"));
			MoveToTarget(CombatTarget);
		}
	}
}

void AEnemy::Attack()
{
	if (EnemyState == EEnemyState::EES_Attacking || EnemyState == EEnemyState::EES_Dead) { return; }
	

	if (AttackMontage && EnemyEquipState == ECharacterState::ECS_Unequipped)
	{
		EnemyState = EEnemyState::EES_Attacking;

		PlayMontage(AttackMontage, FName("Attack1"));
	}
	else if (ArcherAttackMontage && EnemyEquipState == ECharacterState::ECS_EquippedLongbow)
	{
		EnemyState = EEnemyState::EES_Attacking;

		PlayMontage(ArcherAttackMontage, FName("DrawArrow"));
	}
	else if (SwordAttackMontage && EnemyEquipState == ECharacterState::ECS_EquippedOneHanded)
	{
		EnemyState = EEnemyState::EES_Attacking;

		PlayMontage(SwordAttackMontage, FName("Default"));
	}
	else if (ThrustAttackMontage && EnemyEquipState == ECharacterState::ECS_EquippedSwordAndShield)
	{
		EnemyState = EEnemyState::EES_Attacking;

		Tags.Remove(FName("Blocking"));
		PlayMontage(ThrustAttackMontage, FName("Default"));
	}
}

void AEnemy::Block()
{
	if (EnemyState == EEnemyState::EES_Blocking) { return; }

	if (BlockMontage)
	{
		PlayMontage(BlockMontage, FName("Default"));
		EnemyState = EEnemyState::EES_Blocking;
		Tags.AddUnique(FName("Blocking"));
	}
}

void AEnemy::Strafe()
{
	if (CombatTarget && InTargetRange(CombatTarget->GetActorLocation(), CannotStrafeRadius)) { return; }

	int32 Selection = FMath::RandRange(0, 1);
	EnemyState = EEnemyState::EES_Strafing;


	if (HasShield)
	{
		switch (Selection)
		{

		case 0:
			PlayMontage(StrafeMontage, FName("ShieldStrafeLeft"));
			break;

		case 1:
			PlayMontage(StrafeMontage, FName("ShieldStrafeRight"));
			Tags.AddUnique(FName("Blocking"));
			break;

		default:
			break;
		}
	}
	else
	{
		switch (Selection)
		{
		case 0:
			PlayMontage(StrafeMontage, FName("WalkStrafeLeft"));
			break;

		case 1:
			PlayMontage(StrafeMontage, FName("WalkStrafeRight"));
			break;

		default:
			break;
		}
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
			Arrow->FireProjectile(CombatTarget->GetActorLocation(), GetActorLocation());
		}
		
	}
}


bool AEnemy::InTargetRange(AActor* Target, const double& Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

bool AEnemy::InTargetRange(const FVector& Target, const double& Radius)
{
	const double DistanceToTarget = (Target - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void AEnemy::AddInitialTags()
{
	Tags.AddUnique(FName("Hitable"));
	Tags.AddUnique(FName("Enemy"));
	Tags.AddUnique(FName("Lockable"));
}

void AEnemy::InitialiseComponents()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SightRadius = VisualRadius;
	PawnSensingComponent->SetPeripheralVisionAngle(PeripheralVisionAngle);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	HealthBarComponent->SetVisibility(false);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));
}

FVector AEnemy::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) { return FVector(); }

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}
