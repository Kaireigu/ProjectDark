// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDARK_API APotion : public AItem
{
	GENERATED_BODY()

public:

	APotion();
	
private:

	UPROPERTY(EditAnywhere, Category = "Potion Properties")
	int32 Uses = 1;

	UPROPERTY(EditAnywhere, Category = "Potion Properties")
	float HealAmount = 20.f;

public:
	FORCEINLINE const int32& GetNumOfUses() { return Uses; }
	FORCEINLINE const float& GetHealAmount() { return HealAmount; }
	FORCEINLINE void ReduceUsesByOne() { Uses -= 1; }
	FORCEINLINE bool CanUsePotion() { return Uses > 0; }

};
