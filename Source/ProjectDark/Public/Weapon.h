// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:

	AWeapon();

	TArray<AActor*> IgnoredActors;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* StartBoxTraceLocation;

	UPROPERTY(EditAnywhere)
	USceneComponent* EndBoxTraceLocation;

	UPROPERTY(EditAnywhere)
	float WeaponDamage = 20.f;

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetWeaponCollision(const bool& Collision);

};
