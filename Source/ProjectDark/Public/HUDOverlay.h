// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDOverlay.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API UHUDOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthBarPercent(const float& Percent, const float& MaxHealth);
	void SetStaminaBarPercent(const float& Percent, const float& MaxStamina);

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;
};
