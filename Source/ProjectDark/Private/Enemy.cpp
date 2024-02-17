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

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique(FName("Hitable"));
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

	UpdatePatrolTarget();

}

void AEnemy::UpdatePatrolTarget()
{
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

				MoveToTarget(PatrolTarget);
			}
		}
	}
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

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) { return; }

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(TargetRadius);
	EnemyController->MoveTo(MoveRequest);
}

bool AEnemy::InTargetRange(AActor* Target, const double& Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}
