// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "NPC.generated.h"

class USphereComponent;
class IInteractInterface;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API ANPC : public AEnemy
{
	GENERATED_BODY()

public:

	ANPC();

	void Tick(float DeltaTime) override;

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Movement)
	bool bCanChasePlayer = false;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	TArray<FString> NPCDialogueText;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	TArray<float> NPCDialoguePlayRate;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	bool bShouldDieAtEndOfDialogue = false;

private:

	UPROPERTY(EditAnywhere, Category = Components)
	USphereComponent* SphereComponent;

	IInteractInterface* PlayerInteractInterface = nullptr;

	int32 SizeOfNPCDialogue;
	int32 CurrentDialogue = 0;
	bool bHasDialogueStarted = false;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PlayDialogue();

	FTimerHandle DialogueTimerHandle;
	
};
