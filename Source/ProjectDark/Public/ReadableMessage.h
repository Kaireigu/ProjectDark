// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReadableMessage.generated.h"

class USphereComponent;

UCLASS()
class PROJECTDARK_API AReadableMessage : public AActor
{
	GENERATED_BODY()
	
public:	
	AReadableMessage();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Component)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = Component)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = Message)
	FString NotifyTextToDisplay;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	virtual void Tick(float DeltaTime) override;

};
