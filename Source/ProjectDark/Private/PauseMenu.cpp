// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UPauseMenu::SwitchOptionResume()
{
	if (ResumeButtonSelectImage == nullptr || QuitButtonSelectImage == nullptr) { return; }

	ResumeButtonSelectImage->SetVisibility(ESlateVisibility::Visible);
	QuitButtonSelectImage->SetVisibility(ESlateVisibility::Hidden);
	bResumeButtonHighlighted = true;
}

void UPauseMenu::SwitchOptionQuit()
{
	if (ResumeButtonSelectImage == nullptr || QuitButtonSelectImage == nullptr) { return; }

	ResumeButtonSelectImage->SetVisibility(ESlateVisibility::Hidden);
	QuitButtonSelectImage->SetVisibility(ESlateVisibility::Visible);
	bResumeButtonHighlighted = false;
}
