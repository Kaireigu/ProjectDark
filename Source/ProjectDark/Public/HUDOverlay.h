// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDOverlay.generated.h"

class UProgressBar;
class UEditableTextBox;

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
	void SetBossHealthPercent(const float& Percent, const float& BossMaxHealth);
	void SetInteractTextBox(const FString& ItemText);
	void SetBossTextBox(const FString& BossName);
	void SetDialogueTextBox(const FString& DialogueText);
	void ClearInteractTextBox();
	void HideInteractTextBox();
	void ShowInteractTextBox();
	void HideBossBar();
	void ShowBossBar();
	void HideDialogueTextBox();
	void ShowDialogueTextBox();

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InteractTextBox;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* BossHealthBar;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* BossTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* DialogueTextBox;

};
