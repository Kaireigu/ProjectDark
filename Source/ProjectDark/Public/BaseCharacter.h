// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "BaseCharacter.generated.h"

class UAnimMontage;
class AWeapon;
class UAttributes;

UCLASS()
class PROJECTDARK_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetHit(AActor* OtherActor, const FVector& ImpactPoint) override;
	void GetHitWithDamage(const float& DamageAmount, const FVector& ImpactPoint) override;

	virtual void PlayHitReactMontage(const FVector& ImpactPoint);
	void PlayStaggerMontage();
	void PlayBackStab() override;
	FVector GetBackStabLocation() override;
	FVector GetFrontStabLocation() override;
	bool IsAnyMontagePlaying();

	bool IsBehind(const AActor* OtherActor);
	bool IsFacing(const AActor* OtherActor);
	bool EnemyIsFacingMe(const AActor* OtherActor);
	bool IsNotInFront(const AActor* OtherActor);

	void AddBackStabTag();
	void RemoveBackStabTag();

protected:

	virtual void BeginPlay() override;

	void PlayMontage(UAnimMontage* Montage, const FName& SectionName);

	double GetTheta(const FVector& Forward, const FVector& OtherActorLocation);

	virtual void Die();

	bool InTargetRange(AActor* Target, const double& Radius);
	bool InTargetRange(const FVector& Target, const double& Radius);

	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void DisableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void SetDeathValues();

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UAttributes* AttributeComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* FrontStabLocation;

	UPROPERTY(EditAnywhere)
	USceneComponent* BackStabLocation;

private:


public:	


};
