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
#include "Components/CapsuleComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetDefaultControllerValues();

	InitialiseComponents();

	Tags.AddUnique(FName("Hitable"));

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindInputActions(PlayerInputComponent);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialiseSubsystem();

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

void APlayerCharacter::SetWeaponSocketOnEquipping()
{
	if (IsUnequipped())
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
	}
	else if (IsEquippedWithOneHandedWeapon())
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
		CharacterState = ECharacterState::ECS_Unequipped;
	}
}

void APlayerCharacter::EnableWeaponCollision()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollision(true);
	}
}

void APlayerCharacter::DisableWeaponCollision()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollision(false);
	}
}

void APlayerCharacter::SetDefaultControllerValues()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::InitialiseComponents()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, OrientRotationRateYaw, 0.f); // Setting Rotation Rate so player orients quicker.

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
}

void APlayerCharacter::InitialiseSubsystem()
{
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

void APlayerCharacter::BindInputActions(UInputComponent* PlayerInputComponent)
{
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

void APlayerCharacter::PlayMontage(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (ActionState != EActionState::EAS_Unoccupied) { return; }

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

	if (CanEquip())
	{
		PlayMontage(EquipMontage, FName("Equip"));
		ActionState = EActionState::EAS_Equipping; // SetUnoccupied will call from an AnimNotify
	}
	else if (CanUnequip())
	{
		PlayMontage(EquipMontage, FName("Unequip"));
		ActionState = EActionState::EAS_Equipping;
	}

	if (EquippedWeapon == nullptr)
	{
		EquippedWeapon = Cast<AWeapon>(OverlappingItem);

		if (EquippedWeapon)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
			EquippedWeapon->SetOwner(this);
			CharacterState = ECharacterState::ECS_EquippedOneHanded;
		}
	}

}

void APlayerCharacter::Attack(const FInputActionValue& value)
{
	if (IsUnequipped()) { return; }

	if (bCanCombo)
	{
		bComboActive = true;
	}

	if (IsOccupied()) { return; }

	PlayMontage(AttackMontageOneHanded, FName("Attack1"));
	ActionState = EActionState::EAS_Attacking;
}

void APlayerCharacter::RollOrBackStep(const FInputActionValue& value)
{
	if (IsOccupied()) { return; }

	ActionState = EActionState::EAS_Dodging;

	if (IsMoving())
	{
		PlayMontage(RollMontage, FName("Roll1"));
	}
	else if (IsNotMoving())
	{
		PlayMontage(BackStepMontage, FName("Default"));
	}
}

bool APlayerCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

bool APlayerCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool APlayerCharacter::CanEquip()
{
	return EquippedWeapon && IsUnequipped() && IsUnoccupied() && !IsMoving();
}

bool APlayerCharacter::CanUnequip()
{
	return EquippedWeapon && IsEquippedWithOneHandedWeapon() && IsUnoccupied() && !IsMoving();
}

bool APlayerCharacter::IsMoving()
{
	const double GroundSpeed = UKismetMathLibrary::VSizeXY(GetMovementComponent()->Velocity);

	if (GroundSpeed > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayerCharacter::IsNotMoving()
{
	const double GroundSpeed = UKismetMathLibrary::VSizeXY(GetMovementComponent()->Velocity);

	if (GroundSpeed > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool APlayerCharacter::IsEquippedWithOneHandedWeapon()
{
	return CharacterState == ECharacterState::ECS_EquippedOneHanded;
}

bool APlayerCharacter::IsUnequipped()
{
	return CharacterState == ECharacterState::ECS_Unequipped;
}


