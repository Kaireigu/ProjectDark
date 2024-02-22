// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectDarkHUD.h"
#include "HUDOverlay.h"

void AProjectDarkHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();

		if (Controller && OverlayWidgetClass)
		{
			HUDOverlay = CreateWidget<UHUDOverlay>(Controller, OverlayWidgetClass);

			if (HUDOverlay)
			{
				HUDOverlay->AddToViewport();
			}
		}
	}
}
