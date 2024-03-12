// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "HitInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

AProjectile::AProjectile()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Tags.AddUnique(FName("Projectile"));


	ProjectileMovementComponent->InitialSpeed = IntialSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bAutoActivate = false;

	SetLifeSpan(LifeSpan);

}

void AProjectile::FireProjectile(const FVector& Target, const FVector& CurrentLocation)
{
	const FVector DirectionToTarget = Target - CurrentLocation;
	ProjectileMovementComponent->InitialSpeed = IntialSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->Velocity = (DirectionToTarget.GetSafeNormal()) * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Activate();
}

void AProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherComp->ComponentHasTag("LockOnBox")) { return; }

	if (OtherActor->ActorHasTag("PlayerCharacter"))
	{
		IHitInterface* PlayerHitInterface = Cast<IHitInterface>(OtherActor);
		ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);

		if (PlayerHitInterface && HitCharacter)
		{
			if (HitCharacter->ActorHasTag("Blocking")) 
			{ 
				const FVector ForwardVector = HitCharacter->GetActorForwardVector();
				const FVector ToEnemy = (GetActorLocation() - HitCharacter->GetActorLocation()).GetSafeNormal();

				const double CosTheta = FVector::DotProduct(ForwardVector, ToEnemy);
				double Theta = UKismetMathLibrary::Acos(CosTheta);
				Theta = UKismetMathLibrary::RadiansToDegrees(Theta);

				const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToEnemy);

				if (CrossProduct.Z < 0)
				{
					Theta *= -1.f;
				}

				if (Theta > 0.f && Theta <= 90.f)
				{
					Destroy();
					PlayerHitInterface->UseStamina(ProjectileDamage / 2);
					return;
				}
				else if (Theta < 0.f && Theta >= -90.f)
				{
					Destroy();
					PlayerHitInterface->UseStamina(ProjectileDamage / 2);
					return;
				}
			}

			PlayerHitInterface->GetHitWithDamage(ProjectileDamage, GetActorLocation());

			ProjectileMovementComponent->Velocity = FVector::ZeroVector;
			//AttachMeshToSocket(HitCharacter->GetMesh(), FName("ProjectileSocket"));
			Destroy();
		}

	}
}
