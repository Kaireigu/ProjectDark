// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/SphereComponent.h"
#include "InteractInterface.h"
#include "Item.h"

ANPC::ANPC()
{
	Tags.Remove(FName("Enemy"));

	PawnSensingComponent = nullptr;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetGenerateOverlapEvents(true);
}

void ANPC::Tick(float DeltaTime)
{
	if (bCanChasePlayer)
	{
		Super::Tick(DeltaTime);
	}
}

void ANPC::BeginPlay()
{
	EnemyState = EEnemyState::EES_Unoccupied;
	Super::BeginPlay();

	Tags.Remove(FName("Lockable"));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnSphereEndOverlap);

	SizeOfNPCDialogue = NPCDialogueText.Num();
}

void ANPC::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("LockOnBox") || OtherActor->ActorHasTag(FName("PlayerCharacter")) == false) { return; }

	PlayerInteractInterface = Cast<IInteractInterface>(OtherActor);

	if (PlayerInteractInterface && SizeOfNPCDialogue > 0 && bHasDialogueStarted == false)
	{
		GetWorldTimerManager().SetTimer(DialogueTimerHandle, this, &ANPC::PlayDialogue, 0.1f, false);
		bHasDialogueStarted = true;
	}
}

void ANPC::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("LockOnBox")) { return; }

}

void ANPC::PlayDialogue()
{
	if (PlayerInteractInterface == nullptr) { return; }

	if (CurrentDialogue < SizeOfNPCDialogue)
	{
		PlayerInteractInterface->SetDialogueText(NPCDialogueText[CurrentDialogue]);
		GetWorldTimerManager().SetTimer(DialogueTimerHandle, this, &ANPC::PlayDialogue, NPCDialoguePlayRate[CurrentDialogue], false);
		CurrentDialogue += 1;
	}
	else
	{
		CurrentDialogue = 0;
		PlayerInteractInterface->ClearDialogueText();
		GetWorldTimerManager().ClearTimer(DialogueTimerHandle);

		if (bShouldDieAtEndOfDialogue && LootItem)
		{
			GetWorld()->SpawnActor<AItem>(LootItem, GetActorLocation(), GetActorRotation());
			SetDeathValues();
		}
	}
}
