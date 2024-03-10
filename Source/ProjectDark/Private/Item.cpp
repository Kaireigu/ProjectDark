// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetGenerateOverlapEvents(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItem::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	AttachToComponent(InParent, AttachmentRules, InSocketName);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

	PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (PlayerCharacter && PlayerCharacter == OtherActor)
	{
		PlayerCharacter->SetOverlappingItem(this);

		if (ItemState != EItemStatus::EIS_Equipped)
		{
			PlayerCharacter->SetHUDInteractText(InteractText);
		}
		else
		{
			PlayerCharacter->ClearHUDInteractText();
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

	if (PlayerCharacter && PlayerCharacter == OtherActor)
	{
		PlayerCharacter->SetOverlappingItem(nullptr);
		PlayerCharacter->ClearHUDInteractText();

	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::SetItemStateEquipped()
{
	ItemState = EItemStatus::EIS_Equipped;
	SphereComponent->SetGenerateOverlapEvents(false);

	if (PlayerCharacter)
	{
		PlayerCharacter->ClearHUDInteractText();
	}
}

