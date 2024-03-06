// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/BoxComponent.h"
#include "Field/FieldSystemComponent.h"

ABreakableActor::ABreakableActor()
{

	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Box->SetGenerateOverlapEvents(false);

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(GetRootComponent());
	GeometryCollection->SetSimulatePhysics(true);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetNotifyBreaks(true);

	FieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("Field System"));
	FieldSystem->SetupAttachment(GetRootComponent());

	MetaData = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("Meta Data"));
	RadialFallOff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFallOff"));
	RadialFallOff->SetRadialFalloff(DestructionStrength, 1.f, 1.f, 1.f, FieldRadius, GetActorLocation(), EFieldFalloffType::Field_FallOff_None);
}


void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

	Tags.AddUnique("Hitable");
	SetOwner(this);

	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnBroken);

}

void ABreakableActor::OnBroken(const FChaosBreakEvent& BreakEvent)
{
	SetLifeSpan(5.f);

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ABreakableActor::GetHit(AActor* OtherActor, const FVector& ImpactPoint)
{
	if (FieldSystem)
	{
		FieldSystem->ApplyStrainField(true, GetActorLocation(), FieldRadius, DestructionStrength, 1);
	}
}

void ABreakableActor::GetHitWithDamage(const float& DamageAmount, const FVector& ImpactPoint)
{

}

