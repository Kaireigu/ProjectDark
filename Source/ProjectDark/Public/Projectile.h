// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API AProjectile : public AItem
{
	GENERATED_BODY()

public:

	AProjectile();

	void BeginPlay() override;

	void FireProjectile(const FVector& Target, const FVector& CurrentLocation);

protected:

	UPROPERTY(EditAnywhere, Category = Movement)
	float IntialSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = Damage)
	float ProjectileDamage = 10.f;

	UPROPERTY(EditAnywhere)
	float LifeSpan = 30.f;


private:

	UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
