// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"

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

void UHUDOverlay::SetInteractTextBox(const FString& ItemText)
{
	if (InteractTextBox)
	{
		InteractTextBox->SetText(FText::FromString(ItemText));
		ShowInteractTextBox();
	}
}

void UHUDOverlay::ClearInteractTextBox()
{
	HideInteractTextBox();

	if (InteractTextBox)
	{
		InteractTextBox->SetText(FText::FromString(""));
	}
}

void UHUDOverlay::HideInteractTextBox()
{
	if (InteractTextBox)
	{
		InteractTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::ShowInteractTextBox()
{
	if (InteractTextBox)
	{
		InteractTextBox->SetVisibility(ESlateVisibility::Visible);
	}
}
