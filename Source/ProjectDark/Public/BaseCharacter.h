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

	virtual void PlayHitReactMontage(const FVector& ImpactPoint);

	bool IsBehind(const AActor* OtherActor);
	bool IsFacing(const AActor* OtherActor);

protected:

	virtual void BeginPlay() override;

	void PlayMontage(UAnimMontage* Montage, const FName& SectionName);

	double GetTheta(const FVector& Forward, const FVector& OtherActorLocation);

	virtual void Die();

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

private:


public:	


};
