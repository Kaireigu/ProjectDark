// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationScreen.h"
#include "Components/Image.h"

void UNotificationScreen::ShowYouDied()
{
	if (CheckImagesValid() == false) { return; }

	BlackBackgroundImage->SetVisibility(ESlateVisibility::Visible);

	YouDiedImage->SetVisibility(ESlateVisibility::Visible);
	VictoryAchievedImage->SetVisibility(ESlateVisibility::Hidden);
	BonfireLitImage->SetVisibility(ESlateVisibility::Hidden);
}

void UNotificationScreen::ShowVictoryAchieved()
{
	if (CheckImagesValid() == false) { return; }

	BlackBackgroundImage->SetVisibility(ESlateVisibility::Visible);

	YouDiedImage->SetVisibility(ESlateVisibility::Hidden);
	VictoryAchievedImage->SetVisibility(ESlateVisibility::Visible);
	BonfireLitImage->SetVisibility(ESlateVisibility::Hidden);

	bVictoryAchieved = true;
}

void UNotificationScreen::ShowBonfireLit()
{
	if (CheckImagesValid() == false) { return; }

	BlackBackgroundImage->SetVisibility(ESlateVisibility::Hidden);

	YouDiedImage->SetVisibility(ESlateVisibility::Hidden);
	VictoryAchievedImage->SetVisibility(ESlateVisibility::Hidden);
	BonfireLitImage->SetVisibility(ESlateVisibility::Visible);
}

void UNotificationScreen::HideAll()
{
	if (CheckImagesValid() == false) { return; }

	BlackBackgroundImage->SetVisibility(ESlateVisibility::Hidden);

	YouDiedImage->SetVisibility(ESlateVisibility::Hidden);
	VictoryAchievedImage->SetVisibility(ESlateVisibility::Hidden);
	BonfireLitImage->SetVisibility(ESlateVisibility::Hidden);
}

bool UNotificationScreen::CheckImagesValid()
{
	if (BlackBackgroundImage != nullptr && YouDiedImage != nullptr && VictoryAchievedImage != nullptr && BonfireLitImage != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
