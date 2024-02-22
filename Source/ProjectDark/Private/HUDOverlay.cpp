// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDOverlay.h"
#include "Components/ProgressBar.h"

void UHUDOverlay::SetHealthBarPercent(const float& Percent, const float& MaxHealth)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(FMath::Clamp(Percent, 0.f, MaxHealth));
	}
}

void UHUDOverlay::SetStaminaBarPercent(const float& Percent, const float& MaxStamina)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(FMath::Clamp(Percent, 0.f, MaxStamina));
	}
}
