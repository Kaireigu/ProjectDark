// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractNode.generated.h"

class IInteractInterface;
class UBoxComponent;
class USoundBase;
class AEnemy;

UCLASS()
class PROJECTDARK_API AInteractNode : public AActor
{
	GENERATED_BODY()
	
public:	

	AInteractNode();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Node Properties")
	bool bIsBossHere = false;

private:

	UFUNCTION()
	void RemoveBossInteraction(AActor* Actor);

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* BossMusic;

	UAudioComponent* BossMusicAudioComponent;

	bool bIsBossMusicPlaying = false;

	IInteractInterface* PlayerInteractInterface;

	UPROPERTY(VisibleInstanceOnly)
	AEnemy* Boss;

public:	

};
