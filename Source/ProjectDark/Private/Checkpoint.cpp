// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "InteractInterface.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;
	InteractText = FString("Rest At Checkpoint");

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

	UGameplayStatics::PlaySoundAtLocation(this, BonfireSound, GetActorLocation());
	
}

void ACheckpoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

	IInteractInterface* InteractInterface = Cast<IInteractInterface>(OtherActor);

	if (InteractInterface)
	{
		InteractInterface->InteractWithCheckpoint();
	}
}

void ACheckpoint::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

	IInteractInterface* InteractInterface = Cast<IInteractInterface>(OtherActor);

	if (InteractInterface)
	{
		InteractInterface->StopInteractionWithCheckpoint();
	}

}

