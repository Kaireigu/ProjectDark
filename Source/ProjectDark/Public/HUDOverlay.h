// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDOverlay.generated.h"

class UProgressBar;
class UEditableTextBox;
class UImage;

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

	void SetNotifyTextBox(const FString& NotifyText);
	void ClearNotifyTextBox();
	void HideNotifyTextBox();
	void ShowNotifyTextBox();

	void SetItemUsesTextBox(const FString& NumberOfUses);
	void ClearItemUsesTextBox();
	void HideItemUsesTextBox();
	void ShowItemUsesTextBox();

	void SetSwordVisible();
	void SetPotionVisible();
	void SetShieldVisible();
	void SetDaggerVisible();
	void HideDagger();
	void HideSword();
	void HideShield();
	void HidePotion();
	void HideAllItems();

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

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* NotifyTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ItemUsesTextBox;

	UPROPERTY(meta = (BindWidget))
	UImage* SwordImage;

	UPROPERTY(meta = (BindWidget))
	UImage* PotionImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ShieldImage;

	UPROPERTY(meta = (BindWidget))
	UImage* DaggerImage;

	bool bDaggerDisplayed = false;

public:

	FORCEINLINE bool& IsDaggerDisplayed() { return bDaggerDisplayed; }

};
