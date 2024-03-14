// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Ladder.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTDARK_API ALadder : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();

	void SetLadderInUse() override;
	void SetLadderNotInUse() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* BottomBox;

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* TopBox;

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* BlockBox;

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* FrontBlockBox;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* LadderStartClimbPosition;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* LadderEndClimbPosition;

private:

	UFUNCTION()
	void OnBottomBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBottomBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnTopBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTopBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

};
