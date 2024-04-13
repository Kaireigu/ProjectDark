// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Enemy.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetHiddenInGame(true);
	SetRootComponent(StaticMesh);

	SpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Position"));
	SpawnPosition->SetupAttachment(GetRootComponent());


}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnEnemy();
	
}

bool ASpawner::CanSpawnEnemy()
{
	if (SpawnedEnemy || EnemyType == nullptr || SpawnPosition == nullptr) { return false; }

	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	TraceStart.Z += 50.f;
	FVector TraceEnd = GetActorLocation() + GetActorUpVector() * -150.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Do a Line Trace to Check if there is something beneath the Spawner to spawn the enemy on.
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);

	if (Hit.GetActor())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASpawner::SpawnEnemy()
{
	UWorld* World = GetWorld();

	if (World && CanSpawnEnemy())
	{
		SpawnedEnemy = World->SpawnActor<AEnemy>(EnemyType, SpawnPosition->GetComponentLocation(), GetActorRotation());
		
		if (SpawnedEnemy)
		{
			SpawnedEnemy->EnemyDied.AddUniqueDynamic(this, &ASpawner::SetNoEnemySpawned);
		}
	}
}

void ASpawner::SetNoEnemySpawned(AActor* Enemy)
{
	SpawnedEnemy = nullptr;
	SpawnEnemy();

}

