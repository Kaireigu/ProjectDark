// Fill out your copyright notice in the Description page of Project Settings.


#include "HitInterface.h"

// Add default functionality here for any IHitInterface functions that are not pure virtual.

void IHitInterface::BeLockedOnTo()
{

}

void IHitInterface::BeLockedOff()
{

}

void IHitInterface::UseStamina(const float& StaminaAmount)
{

}

void IHitInterface::RechargeStamina()
{

}

void IHitInterface::PlayBackStab()
{

}

void IHitInterface::InterfacePlayHitReact(const FVector& ImpactPoint)
{
}

FVector IHitInterface::GetBackStabLocation()
{
	return FVector::ZeroVector;
}

FVector IHitInterface::GetFrontStabLocation()
{
	return FVector::ZeroVector;
}

void IHitInterface::PlayHitShieldSound()
{

}

void IHitInterface::PlayHeadHitMontage()
{

}

void IHitInterface::SetHitBossHeadLandPosition(const FVector& HitPosition)
{

}

FVector IHitInterface::GetHitInTheHeadPosition()
{
	return FVector::ZeroVector;
}
