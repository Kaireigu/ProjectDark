// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"

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

void UHUDOverlay::SetBossHealthPercent(const float& Percent, const float& BossMaxHealth)
{
	if (BossHealthBar)
	{
		BossHealthBar->SetPercent(FMath::Clamp(Percent, 0.f, BossMaxHealth));
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

void UHUDOverlay::SetBossTextBox(const FString& BossName)
{
	if (BossTextBox)
	{
		BossTextBox->SetText(FText::FromString(BossName));
		ShowBossBar();
	}
}

void UHUDOverlay::SetDialogueTextBox(const FString& DialogueText)
{
	if (DialogueTextBox)
	{
		DialogueTextBox->SetText(FText::FromString(DialogueText));
		ShowDialogueTextBox();
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

void UHUDOverlay::HideBossBar()
{
	if (BossTextBox && BossHealthBar)
	{
		BossTextBox->SetVisibility(ESlateVisibility::Hidden);
		BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::ShowBossBar()
{
	if (BossTextBox)
	{
		BossTextBox->SetVisibility(ESlateVisibility::Visible);
		BossHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDOverlay::HideDialogueTextBox()
{
	if (DialogueTextBox)
	{
		DialogueTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::ShowDialogueTextBox()
{
	if (DialogueTextBox)
	{
		DialogueTextBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDOverlay::SetNotifyTextBox(const FString& NotifyText)
{
	if (NotifyTextBox)
	{
		NotifyTextBox->SetText(FText::FromString(NotifyText));
	}
}

void UHUDOverlay::ClearNotifyTextBox()
{
	HideNotifyTextBox();

	if (NotifyTextBox)
	{
		NotifyTextBox->SetText(FText::FromString(""));
	}
}

void UHUDOverlay::HideNotifyTextBox()
{
	if (NotifyTextBox)
	{
		NotifyTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::ShowNotifyTextBox()
{
	if (NotifyTextBox)
	{
		NotifyTextBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDOverlay::SetItemUsesTextBox(const FString& NumberOfUses)
{
	if (ItemUsesTextBox)
	{
		ItemUsesTextBox->SetText(FText::FromString(NumberOfUses));
	}
}

void UHUDOverlay::ClearItemUsesTextBox()
{
	if (ItemUsesTextBox)
	{
		ItemUsesTextBox->SetText(FText::FromString(""));
	}
}

void UHUDOverlay::HideItemUsesTextBox()
{
	if (ItemUsesTextBox)
	{
		ItemUsesTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::ShowItemUsesTextBox()
{
	if (ItemUsesTextBox)
	{
		ItemUsesTextBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDOverlay::SetSwordVisible()
{
	if (SwordImage)
	{
		SwordImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDOverlay::SetPotionVisible()
{
	if (PotionImage)
	{
		PotionImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDOverlay::SetShieldVisible()
{
	if (ShieldImage)
	{
		ShieldImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDOverlay::SetDaggerVisible()
{
	if (DaggerImage)
	{
		DaggerImage->SetVisibility(ESlateVisibility::Visible);
		bDaggerDisplayed = true;
	}
}

void UHUDOverlay::HideDagger()
{
	if (DaggerImage)
	{
		DaggerImage->SetVisibility(ESlateVisibility::Hidden);
		bDaggerDisplayed = false;
	}
}

void UHUDOverlay::HideSword()
{
	if (SwordImage)
	{
		SwordImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::HideShield()
{
	if (ShieldImage)
	{
		ShieldImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::HidePotion()
{
	if (PotionImage)
	{
		PotionImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDOverlay::HideAllItems()
{
	HideDagger();
	HideSword();
	HideShield();
	HidePotion();
}
