// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelLoader.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALevelLoader::ALevelLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetupAttachment(GetRootComponent());

}

void ALevelLoader::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelLoader::OnBoxBeginOverlap);
}

void ALevelLoader::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

	if (bIsLoadingBox && OtherActor->ActorHasTag(FName("PlayerCharacter")))
	{
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, false, FLatentActionInfo());
	}
	else if (bIsUnloadingBox && OtherActor->ActorHasTag(FName("PlayerCharacter")))
	{
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, FLatentActionInfo(), false);
	}
}


