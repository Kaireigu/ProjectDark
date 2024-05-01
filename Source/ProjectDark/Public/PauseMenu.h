// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UImage* ResumeButtonSelectImage;

	UPROPERTY(meta = (BindWidget))
	UImage* QuitButtonSelectImage;

	void SwitchOptionResume();
	void SwitchOptionQuit();

private:

	bool bResumeButtonHighlighted = true;

public:
	FORCEINLINE bool IsResumeButtonHighlighted() { return bResumeButtonHighlighted; }

};
