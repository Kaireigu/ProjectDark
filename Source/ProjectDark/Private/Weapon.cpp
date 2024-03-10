// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"

AWeapon::AWeapon()
{
	Tags.AddUnique("Weapon");
	InteractText = FString("Pick Up Weapon");

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	if (BoxComponent)
	{
		BoxComponent->SetupAttachment(GetRootComponent());
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	StartBoxTraceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Start Trace Location"));

	if (StartBoxTraceLocation)
	{
		StartBoxTraceLocation->SetupAttachment(GetRootComponent());
	}

	EndBoxTraceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("End Trace Location"));

	if (EndBoxTraceLocation)
	{
		EndBoxTraceLocation->SetupAttachment(GetRootComponent());
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	const FVector Start = StartBoxTraceLocation->GetComponentLocation();
	const FVector End = EndBoxTraceLocation->GetComponentLocation();
	const FVector HalfSize = FVector(5.f, 5.f, 5.f);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	ActorsToIgnore.AddUnique(GetOwner());
	FHitResult HitResult;

	for (AActor* Actor : IgnoredActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, HalfSize, GetActorRotation(), ETraceTypeQuery::TraceTypeQuery1, false, 
		ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (HitResult.GetActor())
	{

		if (HitResult.GetActor()->ActorHasTag(FName("Hitable")))
		{
			if (HitResult.GetActor()->ActorHasTag(FName("Blocking")))
			{
				ABaseCharacter* EnemyCharacter = Cast<ABaseCharacter>(HitResult.GetActor());
				ABaseCharacter* ThisCharacter = Cast<ABaseCharacter>(GetOwner());

				if (EnemyCharacter && ThisCharacter)
				{
					if (EnemyCharacter->IsFacing(GetOwner()))
					{ 
						ThisCharacter->PlayHitReactMontage(HitResult.ImpactPoint);
						return; 
					}
				}
			}

			IHitInterface* HittableActor = Cast<IHitInterface>(HitResult.GetActor());

			if (HittableActor)
			{
				HittableActor->GetHit(GetOwner(), GetOwner()->GetActorLocation());
			}

			UGameplayStatics::ApplyDamage(HitResult.GetActor(), WeaponDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			IgnoredActors.AddUnique(HitResult.GetActor());
		}
	}

}

void AWeapon::SetWeaponCollision(const bool& Collision)
{
	if (BoxComponent)
	{
		if (Collision)
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
