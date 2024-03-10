// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion.h"

APotion::APotion()
{
	Tags.AddUnique(FName("Potion"));
	InteractText = FString("Potion");
}
