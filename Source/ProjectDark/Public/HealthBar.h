// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UImage;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UImage* LockOnSymbol;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* DamageNumberTextBox;

};
