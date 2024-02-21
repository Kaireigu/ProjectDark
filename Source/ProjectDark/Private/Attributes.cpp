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

