// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootSceneComponent);

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ForwardArrow->SetupAttachment(GetRootComponent());

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	LeftDoor->SetupAttachment(GetRootComponent());

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	RightDoor->SetupAttachment(GetRootComponent());

	FrontOfDoorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Box"));
	FrontOfDoorBox->SetupAttachment(GetRootComponent());

	BackOfDoorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Back Box"));
	BackOfDoorBox->SetupAttachment(GetRootComponent());

}

void ADoor::OpenDoor()
{
	if (LeftDoor == nullptr || RightDoor == nullptr || bDoorOpen) { return; }

	LeftDoor->SetWorldRotation(LeftDoorOpenRotation);
	RightDoor->SetWorldRotation(RightDoorOpenRotation);
	bDoorOpen = true;
}

void ADoor::CloseDoor()
{
	if (LeftDoor == nullptr || RightDoor == nullptr) { return; }

	LeftDoor->SetWorldRotation(LeftDoorClosedRotation);
	RightDoor->SetWorldRotation(RightDoorClosedRotation);
	bDoorOpen = false;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (FrontOfDoorBox)
	{
		FrontOfDoorBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnFrontBoxBeginOverlap);
		FrontOfDoorBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnFrontBoxEndOverlap);
	}

	if (BackOfDoorBox)
	{
		BackOfDoorBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBackBoxBeginOverlap);
		BackOfDoorBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnBackBoxEndOverlap);
	}
}

void ADoor::OnFrontBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("LockOnBox") || OtherActor == nullptr) { return; }

	if (OtherActor->ActorHasTag(FName("PlayerCharacter")) && PlayerInteractInterface == nullptr)
	{
		PlayerInteractInterface = Cast<IInteractInterface>(OtherActor);
	}

	if (PlayerInteractInterface)
	{
		PlayerInteractInterface->SetCanOpenDoor(true, this);
	}
}

void ADoor::OnFrontBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("LockOnBox") || OtherActor == nullptr) { return; }

	if (PlayerInteractInterface)
	{
		PlayerInteractInterface->SetCanOpenDoor(false, this);
	}
}

void ADoor::OnBackBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("LockOnBox") || OtherActor == nullptr) { return; }

	if (OtherActor->ActorHasTag(FName("PlayerCharacter")))
	{
		CloseDoor();
	}
}

void ADoor::OnBackBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("LockOnBox") || OtherActor == nullptr) { return; }

}

