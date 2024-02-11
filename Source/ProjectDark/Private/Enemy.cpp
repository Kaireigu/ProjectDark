// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique(FName("Hitable"));

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::GetHit()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString(GetActorNameOrLabel() + ": I got HIT!!"));
	}
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}
