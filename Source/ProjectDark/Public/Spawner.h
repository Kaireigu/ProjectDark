// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AEnemy;

UCLASS()
class PROJECTDARK_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpawner();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Properties)
	USceneComponent* SpawnPosition;

	UPROPERTY(EditAnywhere, Category = Properties)
	TSubclassOf<AEnemy> EnemyType;

	UPROPERTY(EditAnywhere, Category = Properties)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleInstanceOnly)
	AEnemy* SpawnedEnemy;

private:

	bool CanSpawnEnemy();
	void SpawnEnemy();
	void SetNoEnemySpawned(AActor* Enemy);

public:	

};
