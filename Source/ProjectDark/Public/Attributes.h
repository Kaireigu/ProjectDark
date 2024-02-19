// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDARK_API UAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttributes();

protected:

	// Current Health
	UPROPERTY(EditInstanceOnly, Category = Attributes)
	float Health = 100.f;

	UPROPERTY(EditInstanceOnly, Category = Attributes)
	float MaxHealth = 100.f;
	
private:



public:	
	FORCEINLINE bool IsAlive() { return Health > 0.f; }

		
};
