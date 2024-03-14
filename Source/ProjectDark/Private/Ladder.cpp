// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"
#include "Components/BoxComponent.h"

// Sets default values
ALadder::ALadder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);

	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bottom Box"));
	BottomBox->SetupAttachment(GetRootComponent());

	TopBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Top Box"));
	TopBox->SetupAttachment(GetRootComponent());

	BlockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Back Collision Box"));
	BlockBox->SetupAttachment(GetRootComponent());
	BlockBox->SetGenerateOverlapEvents(false);
	BlockBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BlockBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	FrontBlockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Collision Box"));
	FrontBlockBox->SetupAttachment(GetRootComponent());
	FrontBlockBox->SetGenerateOverlapEvents(false);
	FrontBlockBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	LadderStartClimbPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Ladder Start Climb Position"));
	LadderStartClimbPosition->SetupAttachment(GetRootComponent());

	LadderEndClimbPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Ladder End Climb Position"));
	LadderEndClimbPosition->SetupAttachment(GetRootComponent());

}

void ALadder::SetLadderInUse()
{
	FrontBlockBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ALadder::SetLadderNotInUse()
{
	FrontBlockBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();

	if (BottomBox)
	{
		BottomBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnBottomBoxBeginOverlap);
		BottomBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnBottomBoxEndOverlap);
	}

	if (TopBox)
	{
		TopBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnTopBoxBeginOverlap);
		TopBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnTopBoxEndOverlap);
	}
	
}

void ALadder::OnBottomBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("LockOnBox"))) { return; }

	if (OtherActor)
	{
		IInteractInterface* Player = Cast<IInteractInterface>(OtherActor);

		if (Player)
		{
			Player->SetCanGetOnLadder(true, GetActorLocation(), LadderStartClimbPosition->GetComponentLocation(), LadderStartClimbPosition->GetComponentRotation(), this);
		}
	}
}

void ALadder::OnBottomBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("LockOnBox"))) { return; }

	if (OtherActor)
	{
		IInteractInterface* Player = Cast<IInteractInterface>(OtherActor);

		if (Player)
		{
			Player->SetCanGetOnLadder(false, GetActorLocation(), LadderStartClimbPosition->GetComponentLocation(), LadderStartClimbPosition->GetComponentRotation(), this);
		}
	}
}

void ALadder::OnTopBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("LockOnBox"))) { return; }

	if (OtherActor)
	{
		IInteractInterface* Player = Cast<IInteractInterface>(OtherActor);

		if (Player)
		{
			Player->SetCanGetOffLadder(true, GetActorLocation(), LadderEndClimbPosition->GetComponentLocation(), LadderEndClimbPosition->GetComponentRotation(), this);
		}
	}
}

void ALadder::OnTopBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("LockOnBox"))) { return; }

	if (OtherActor)
	{
		IInteractInterface* Player = Cast<IInteractInterface>(OtherActor);

		if (Player)
		{
			Player->SetCanGetOffLadder(false, GetActorLocation(), LadderEndClimbPosition->GetComponentLocation(), LadderEndClimbPosition->GetComponentRotation(), this);
		}
	}
}

