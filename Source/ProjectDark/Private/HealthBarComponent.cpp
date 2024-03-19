// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(const float& Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}

void UHealthBarComponent::HideLockOnSymbol()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->LockOnSymbol->SetVisibility(ESlateVisibility::Hidden);
	}
}
