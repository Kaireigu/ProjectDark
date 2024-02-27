// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UBoxComponent;
class UFieldSystemComponent;
class UFieldSystemMetaDataFilter;
class URadialFalloff;
class URadialVector;

UCLASS()
class PROJECTDARK_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	

	ABreakableActor();

	UFUNCTION()
	void OnBroken(const FChaosBreakEvent& BreakEvent);

	virtual void GetHit(AActor* OtherActor, const FVector& ImpactPoint) override;

	UPROPERTY(EditAnywhere)
	float DestructionStrength = 50.f;

	UPROPERTY(EditAnywhere)
	float FieldRadius = 50.f;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere)
	UFieldSystemComponent* FieldSystem;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	UFieldSystemMetaDataFilter* MetaData;

	UPROPERTY(EditAnywhere)
	URadialFalloff* RadialFallOff;

	UPROPERTY(EditAnywhere)
	URadialVector* RadialVector;

private:


public:	

};
