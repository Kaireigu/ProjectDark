// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractInterface.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.

void IInteractInterface::InteractWithCheckpoint()
{

}

void IInteractInterface::SetCanGetOnLadder(const bool& CanGetOn, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder)
{
}

void IInteractInterface::SetCanGetOffLadder(const bool& CanGetOff, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder)
{
}

void IInteractInterface::SetLadderInUse()
{

}

void IInteractInterface::SetLadderNotInUse()
{

}

void IInteractInterface::SetupBossBar(const FString& BossName, const float& BossMaxHealth)
{

}

void IInteractInterface::HideBossBar()
{

}

void IInteractInterface::UpdateBossBar(const float& BossCurrentHealth)
{

}

void IInteractInterface::SetDialogueText(const FString& TextToDisplay)
{

}

void IInteractInterface::ClearDialogueText()
{

}
