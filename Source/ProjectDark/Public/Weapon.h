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

	UPROPERTY(EditAnywhere, Category = Properties)
	float WeaponDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = Properties)
	float BackStabDamage = 50.f;

	UPROPERTY(EditAnywhere, Category = Properties)
	FVector HalfSize = FVector(5.f, 5.f, 5.f);

	UPROPERTY(EditAnywhere, Category = Properties)
	bool bIsBossWeapon = false;

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetWeaponCollision(const bool& Collision);
	FORCEINLINE const float& GetWeaponDamage() { return WeaponDamage; }
	FORCEINLINE const float& GetBackStabDamage() { return BackStabDamage; }

};
