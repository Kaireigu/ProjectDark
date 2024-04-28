// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationScreen.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTDARK_API UNotificationScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void ShowYouDied();
	void ShowVictoryAchieved();
	void ShowBonfireLit();
	void HideAll();

protected:

	UPROPERTY(meta = (BindWidget))
	UImage* BlackBackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UImage* YouDiedImage;

	UPROPERTY(meta = (BindWidget))
	UImage* VictoryAchievedImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BonfireLitImage;

	UPROPERTY(EditAnywhere, Category = Properties)
	float DisplayTimer = 2.f;

private:

	bool CheckImagesValid();
	bool bVictoryAchieved = false;

public:

	FORCEINLINE float& GetDisplayTime() { return DisplayTimer; }
	FORCEINLINE bool& IsVictoryAchieved() { return bVictoryAchieved; }

};
