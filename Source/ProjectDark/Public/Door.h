// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Door.generated.h"

class UBoxComponent;
class UArrowComponent;

UCLASS()
class PROJECTDARK_API ADoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	

	ADoor();

	void OpenDoor() override;
	void CloseDoor();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFrontBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFrontBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBackBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBackBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = Component)
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, Category = Component)
	UArrowComponent* ForwardArrow;

	UPROPERTY(EditAnywhere, Category = Component)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere, Category = Component)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, Category = Component)
	UBoxComponent* FrontOfDoorBox;

	UPROPERTY(EditAnywhere, Category = Component)
	UBoxComponent* BackOfDoorBox;

	UPROPERTY(EditAnywhere, Category = Properties)
	FRotator LeftDoorClosedRotation;

	UPROPERTY(EditAnywhere, Category = Properties)
	FRotator RightDoorClosedRotation;

	UPROPERTY(EditAnywhere, Category = Properties)
	FRotator LeftDoorOpenRotation;

	UPROPERTY(EditAnywhere, Category = Properties)
	FRotator RightDoorOpenRotation;

	UPROPERTY(EditAnywhere, Category = Properties)
	bool bDoorOpen = false;

	IInteractInterface* PlayerInteractInterface;

};
