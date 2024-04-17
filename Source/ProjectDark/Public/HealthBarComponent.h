// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	void SetHealthPercent(const float& Percent);
	void HideLockOnSymbol();

	void SetDamageNumberTextBox(const int32& DamageReceived);
	void ClearDamageNumberTextBox();
	void ShowDamageNumberTextBox();
	void HideDamageNumberTextBox();
	void SetClearDamageNumberTextBoxTimer();

protected:

	FTimerHandle ClearDamageTextBoxTimerHandle;

private:

	UPROPERTY()
	UHealthBar* HealthBarWidget;

	int32 DamageReceivedTotal = 0;
};
