// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

};
