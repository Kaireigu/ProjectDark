// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/Actor.h"

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
	if (HealthBarWidget && HealthBarWidget->LockOnSymbol)
	{
		HealthBarWidget->LockOnSymbol->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHealthBarComponent::SetDamageNumberTextBox(const int32& DamageReceived)
{
	if (HealthBarWidget && HealthBarWidget->DamageNumberTextBox)
	{
		DamageReceivedTotal += DamageReceived;
		HealthBarWidget->DamageNumberTextBox->SetText(FText::FromString(FString::FromInt(DamageReceivedTotal)));
		ShowDamageNumberTextBox();
		SetClearDamageNumberTextBoxTimer();
	}
}

void UHealthBarComponent::ClearDamageNumberTextBox()
{
	if (HealthBarWidget && HealthBarWidget->DamageNumberTextBox)
	{
		HealthBarWidget->DamageNumberTextBox->SetText(FText::FromString(""));
		HideDamageNumberTextBox();
		DamageReceivedTotal = 0;
	}
}

void UHealthBarComponent::ShowDamageNumberTextBox()
{
	if (HealthBarWidget && HealthBarWidget->DamageNumberTextBox)
	{
		HealthBarWidget->DamageNumberTextBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHealthBarComponent::HideDamageNumberTextBox()
{
	if (HealthBarWidget && HealthBarWidget->DamageNumberTextBox)
	{
		HealthBarWidget->DamageNumberTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHealthBarComponent::SetClearDamageNumberTextBoxTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ClearDamageTextBoxTimerHandle, this, &UHealthBarComponent::ClearDamageNumberTextBox, 2.f);
}
