// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	StartBoxTraceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Start Trace Location"));
	StartBoxTraceLocation->SetupAttachment(GetRootComponent());

	EndBoxTraceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("End Trace Location"));
	EndBoxTraceLocation->SetupAttachment(GetRootComponent());
}

void AWeapon::Equip(USceneComponent* InParent, const FName& InSocketName)
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString("Overlapped With ") + OtherActor->GetName());
	}
}
