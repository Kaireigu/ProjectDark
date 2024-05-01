// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "InteractInterface.h"
#include "Checkpoint.generated.h"

class UCapsuleComponent;
class USoundBase;
class UCascadeParticleSystemComponent;

UCLASS()
class PROJECTDARK_API ACheckpoint : public AItem, public IInteractInterface
{
	GENERATED_BODY()
	
public:

	ACheckpoint();
	virtual void Tick(float DeltaTime) override;

	void LightBonfire() override;

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* BonfireSound;

	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* FireVFX;

private:

	bool bBonfireLit = false;

public:

};
