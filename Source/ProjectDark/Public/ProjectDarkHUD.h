// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectDarkHUD.generated.h"

class UHUDOverlay;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API AProjectDarkHUD : public AHUD
{
	GENERATED_BODY()

public:

	void BeginPlay() override;

	UHUDOverlay* HUDOverlay;

protected:

	UPROPERTY(EditDefaultsOnly, Category = ProjectDark)
	TSubclassOf<UHUDOverlay> OverlayWidgetClass;
	
public:

	FORCEINLINE UHUDOverlay* GetHUDOverlay() { return HUDOverlay; }

};
