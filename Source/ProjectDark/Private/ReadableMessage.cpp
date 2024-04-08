// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadableMessage.h"
#include "Components/SphereComponent.h"
#include "InteractInterface.h"

// Sets default values
AReadableMessage::AReadableMessage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AReadableMessage::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AReadableMessage::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AReadableMessage::OnSphereEndOverlap);
	
}

// Called every frame
void AReadableMessage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReadableMessage::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("LockOnBox"))) { return; }

	if (OtherActor && OtherActor->ActorHasTag(FName("PlayerCharacter")))
	{
		IInteractInterface* PlayerInterface = Cast<IInteractInterface>(OtherActor);

		if (PlayerInterface == nullptr) { return; }

		PlayerInterface->SetHUDInteractText(FString("Read Message"));
		PlayerInterface->SetHUDNotifyText(NotifyTextToDisplay);
	}
}

void AReadableMessage::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("LockOnBox"))) { return; }

	if (OtherActor && OtherActor->ActorHasTag(FName("PlayerCharacter")))
	{
		IInteractInterface* PlayerInterface = Cast<IInteractInterface>(OtherActor);

		if (PlayerInterface == nullptr) { return; }

		PlayerInterface->ClearHUDInteractText();
		PlayerInterface->ClearHUDNotifyText();
	}
}

