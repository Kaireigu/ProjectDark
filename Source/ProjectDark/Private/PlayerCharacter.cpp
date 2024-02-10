// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item.h"
#include "Weapon.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, OrientRotationRateYaw, 0.f); // Setting Rotation Rate so player orients quicker.

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 1);
		}
	}
	
}

void APlayerCharacter::SetCanCombo(bool CanCombo)
{
	bCanCombo = CanCombo;
}

void APlayerCharacter::AttackEnd()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bComboActive)
	{
		bComboActive = false;

	}
	else
	{
		ActionState = EActionState::EAS_Unoccupied;
		AnimInstance->StopAllMontages(0.25f);
	}
}

void APlayerCharacter::SetUnoccupied()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	const FVector2D Movement = value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator YawRotation = FRotator(0.f, ControllerRotation.Yaw, 0.f);

		FVector Forward = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Forward, Movement.X);

		FVector Right = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Right, Movement.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookInput.X);
		AddControllerPitchInput(LookInput.Y);
	}
}

void APlayerCharacter::EKeyPressed(const FInputActionValue& value)
{

	if (EquippedWeapon == nullptr)
	{
		EquippedWeapon = Cast<AWeapon>(OverlappingItem);
	}

	if (EquippedWeapon && CharacterState == ECharacterState::ECS_Unequipped)
	{
		EquippedWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
	}

}

void APlayerCharacter::Attack(const FInputActionValue& value)
{
	if (CharacterState == ECharacterState::ECS_Unequipped) { return; }

	if (bCanCombo)
	{
		bComboActive = true;
	}

	if (ActionState != EActionState::EAS_Unoccupied) { return; }

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontageOneHanded)
	{
		AnimInstance->Montage_Play(AttackMontageOneHanded);
		AnimInstance->Montage_JumpToSection(FName("Attack1"), AttackMontageOneHanded);
		ActionState = EActionState::EAS_Attacking;
	}
}

void APlayerCharacter::RollOrBackStep(const FInputActionValue& value)
{
	if (ActionState != EActionState::EAS_Unoccupied) { return; }

	ActionState = EActionState::EAS_Dodging;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && RollMontage && UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) > 0.f)
	{
		AnimInstance->Montage_Play(RollMontage);
		AnimInstance->Montage_JumpToSection(FName("Roll1"), RollMontage);
	}
	else if (AnimInstance && BackStepMontage && UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) == 0.f)
	{
		AnimInstance->Montage_Play(BackStepMontage);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}

		if (EPressedAction)
		{
			EnhancedInputComponent->BindAction(EPressedAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EKeyPressed);
		}

		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
		}

		if (RollOrBackStepAction)
		{
			EnhancedInputComponent->BindAction(RollOrBackStepAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RollOrBackStep);
		}

	}

}

