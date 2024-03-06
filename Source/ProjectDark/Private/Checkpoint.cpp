// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "InteractInterface.h"
#include "Components/CapsuleComponent.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->SetupAttachment(GetRootComponent());
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACheckpoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

	IInteractInterface* InteractInterface = Cast<IInteractInterface>(OtherActor);

	if (InteractInterface)
	{
		InteractInterface->InteractWithCheckpoint();
	}
}

void ACheckpoint::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

	IInteractInterface* InteractInterface = Cast<IInteractInterface>(OtherActor);

	if (InteractInterface)
	{
		InteractInterface->InteractWithCheckpoint();
	}

}

