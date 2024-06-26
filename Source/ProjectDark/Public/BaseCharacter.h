// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "BaseCharacter.generated.h"

class UAnimMontage;
class AWeapon;
class UAttributes;
class USoundBase;
class UParticleSystem;

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
	virtual void PlayLargeHitReactMontage();
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
	void PlayHitShieldSound() override;

protected:

	virtual void BeginPlay() override;

	void PlayMontage(UAnimMontage* Montage, const FName& SectionName);
	void StopAllMontages();

	double GetTheta(const FVector& Forward, const FVector& OtherActorLocation);
	double GetThetaFromActors(const AActor* ForwardVectorActor, const AActor* LocationActor);

	virtual void Die();

	bool InTargetRange(AActor* Target, const double& Radius);
	bool InTargetRange(const FVector& Target, const double& Radius);

	UFUNCTION(BlueprintCallable)
	virtual void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	virtual void DisableWeaponCollision();

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

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* StaggeredSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* ShieldHitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

private:


public:	


};
