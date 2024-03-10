// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class APlayerCharacter;

enum class EItemStatus : uint8
{
	EIS_Unequipped,
	EIS_Equipped
};

UCLASS()
class PROJECTDARK_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = "HUD")
	FString InteractText = FString("Text Not Set");

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	EItemStatus ItemState = EItemStatus::EIS_Unequipped;

	APlayerCharacter* PlayerCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetItemStateEquipped();
	FORCEINLINE bool IsItemUnequipped() { return ItemState == EItemStatus::EIS_Unequipped; }

};
