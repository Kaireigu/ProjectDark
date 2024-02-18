// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"

AWeapon::AWeapon()
{
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

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	AttachToComponent(InParent, AttachmentRules, InSocketName);
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

	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, HalfSize, GetActorRotation(), ETraceTypeQuery::TraceTypeQuery1, false, 
		ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (HitResult.GetActor())
	{

		if (HitResult.GetActor()->ActorHasTag(FName("Hitable")))
		{

			IHitInterface* HittableActor = Cast<IHitInterface>(HitResult.GetActor());

			if (HittableActor)
			{
				HittableActor->GetHit(GetOwner(), GetOwner()->GetActorLocation());
			}

			ActorsToIgnore.AddUnique(HitResult.GetActor());
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
