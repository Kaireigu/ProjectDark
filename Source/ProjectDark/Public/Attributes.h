// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTDARK_API UAttributes : public UActorComponent
{
	GENERATED_BODY()

public:

	UAttributes();

	void ReceiveDamage(const float& Damage);
	void ReceiveHeal(const float& HealAmount);
	void UseStamina(const float& StaminaAmount);
	void StartStaminaRecharge();
	void StopStaminaRecharge();
	void StartDrainStamina();
	void StopDrainStamina();
	void RechargeStamina(const float& DeltaTime);
	void DrainStamina(const float& DeltaTime);
	void SetInitialHealthAndStaminaValues();

protected:

	// Current Health
	UPROPERTY(EditAnywhere, Category = Attributes)
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float StaminaRechargeRate = 20.f;

private:

	bool bShouldRechargeStamina = false;
	bool bShouldDrainStamina = false;

public:
	FORCEINLINE bool IsAlive() { return Health > 0.f; }
	FORCEINLINE float GetHealthPercent() { return Health / MaxHealth; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetStaminaPercent() { return Stamina / MaxStamina; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetCurrentStamina() { return Stamina; }

};
