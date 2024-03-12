// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes.h"

// Sets default values for this component's properties
UAttributes::UAttributes()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttributes::ReceiveDamage(const float& Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttributes::ReceiveHeal(const float& HealAmount)
{
	Health = FMath::Clamp(Health + HealAmount, 0.f, MaxHealth);
}

void UAttributes::UseStamina(const float& StaminaAmount)
{
	Stamina = FMath::Clamp(Stamina - StaminaAmount, 0.f, MaxStamina);
}

void UAttributes::StartStaminaRecharge()
{
	bShouldRechargeStamina = true;
}

void UAttributes::StopStaminaRecharge()
{
	bShouldRechargeStamina = false;
}

void UAttributes::RechargeStamina(const float& DeltaTime)
{
	if (bShouldRechargeStamina)
	{
		Stamina += FMath::Clamp(StaminaRechargeRate * DeltaTime, 0, MaxStamina);
		GetOwner()->Tags.AddUnique(FName("RechargingStamina"));

		if (Stamina >= MaxStamina)
		{
			bShouldRechargeStamina = false;
			GetOwner()->Tags.Remove(FName("RechargingStamina"));
		}
	}
}

void UAttributes::SetInitialHealthAndStaminaValues()
{
	Health = MaxHealth;
	Stamina = MaxStamina;
}

