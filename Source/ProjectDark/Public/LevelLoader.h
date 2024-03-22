// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelLoader.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTDARK_API ALevelLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelLoader();

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Components)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = Levels)
	FName LevelToLoad;

	UPROPERTY(EditAnywhere, Category = Levels)
	FName LevelToUnload;

	UPROPERTY(EditAnywhere, Category = Levels)
	bool bIsLoadingBox;

	UPROPERTY(EditAnywhere, Category = Levels)
	bool bIsUnloadingBox;

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
