// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthPercent(const float& Percent);

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
};
