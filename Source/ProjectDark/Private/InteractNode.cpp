

#include "InteractNode.h"
#include "InteractInterface.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Enemy.h"

AInteractNode::AInteractNode()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

}

void AInteractNode::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractNode::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractNode::OnBoxEndOverlap);
	
}

void AInteractNode::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("LockOnBox") || OtherActor->ActorHasTag(FName("PlayerCharacter")) == false) { return; }

	
	TArray<AActor*> OverlappingActors;
	BoxComponent->GetOverlappingActors(OverlappingActors);

	for (int i = 0; i < OverlappingActors.Num(); i++)
	{
		if (OverlappingActors[i]->ActorHasTag(FName("Boss")))
		{
			bIsBossHere = true;
			Boss = Cast<AEnemy>(OverlappingActors[i]);
		}
	}

	if (Boss)
	{
		Boss->EnemyDied.AddUniqueDynamic(this, &AInteractNode::RemoveBossInteraction);
	}

	if (bIsBossHere && BossMusic)
	{
		if (PlayerInteractInterface == nullptr)
		{
			PlayerInteractInterface = Cast<IInteractInterface>(OtherActor);
		}

		if (bIsBossMusicPlaying == false && PlayerInteractInterface)
		{
			BossMusicAudioComponent = UGameplayStatics::SpawnSoundAttached(BossMusic, GetRootComponent());
			bIsBossMusicPlaying = true;
			PlayerInteractInterface->ShowBossBar();
		}
	}
}

void AInteractNode::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("LockOnBox") || OtherActor->ActorHasTag(FName("PlayerCharacter")) == false) { return; }

	PlayerInteractInterface = Cast<IInteractInterface>(OtherActor);

	RemoveBossInteraction(this);
}

void AInteractNode::RemoveBossInteraction(AActor* Actor)
{
	if (BossMusicAudioComponent && PlayerInteractInterface)
	{
		bIsBossMusicPlaying = false;

		if (BossMusicAudioComponent->IsPlaying())
		{
			BossMusicAudioComponent->Stop();
			BossMusicAudioComponent = nullptr;
		}

		PlayerInteractInterface->HideBossBar();
		bIsBossHere = false;
	}
}

