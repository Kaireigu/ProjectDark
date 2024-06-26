// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTDARK_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void InteractWithCheckpoint(const bool& BonfireLit, IInteractInterface* Bonfire);
	virtual void LightBonfire();
	virtual void StopInteractionWithCheckpoint();
	virtual void SetCanGetOnLadder(const bool& CanGetOn, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder);
	virtual void SetCanGetOffLadder(const bool& CanGetOff, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder);
	virtual void SetLadderInUse();
	virtual void SetLadderNotInUse();
	virtual void SetupBossBar(const FString& BossName, const float& BossMaxHealth);
	virtual void HideBossBar();
	virtual void ShowBossBar();
	virtual void UpdateBossBar(const float& BossCurrentHealth);
	virtual void SetDialogueText(const FString& TextToDisplay);
	virtual void ClearDialogueText();
	virtual void SetHUDInteractText(const FString& InteractText);
	virtual void ClearHUDInteractText();
	virtual void SetHUDNotifyText(const FString& NotifyText);
	virtual void ClearHUDNotifyText();
	virtual void SetCanOpenDoor(const bool& CanOpenDoor, IInteractInterface* Door);
	virtual void OpenDoor();
	virtual void DisplayVictoryAchieved();
};
