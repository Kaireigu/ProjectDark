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
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"

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

	UpdateLockOnTarget(DeltaTime);

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

	LockOnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Lock On Box"));
	LockOnBox->SetupAttachment(Camera);

	StartTraceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	StartTraceLocation->SetupAttachment(Camera);

	EndTraceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	EndTraceLocation->SetupAttachment(Camera);
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

		if (LockOnAction)
		{
			EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LockOn);
		}

		if (SwitchLockedTargetAction)
		{
			EnhancedInputComponent->BindAction(SwitchLockedTargetAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchLockOnTarget);
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
	if (IsOccupied()) { return; }

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
	if (bIsLockingOn) { return; }

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

void APlayerCharacter::LockOn(const FInputActionValue& value)
{
	if (bIsLockingOn)
	{
		bIsLockingOn = false;
		bIsFirstTimeLockingOn = true;
		CurrentEnemyTarget = nullptr;
		EnemyTargetRight = nullptr;
		EnemyTargetLeft = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		bIsLockingOn = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

}

void APlayerCharacter::SwitchLockOnTarget(const FInputActionValue& value)
{
	const float Input = value.Get<float>();

	if (Input > 0.f && EnemyTargetRight)
	{
		CurrentEnemyTarget = EnemyTargetRight;
	}
	else if (Input < 0.f && EnemyTargetLeft)
	{
		CurrentEnemyTarget = EnemyTargetLeft;
	}
}

void APlayerCharacter::LockOnBoxTrace()
{
	// Gets all the enemies within a trace and stores them in Lockable Enemies
	LockableEnemies.Empty();

	const FVector Start = StartTraceLocation->GetComponentLocation();
	const FVector End = EndTraceLocation->GetComponentLocation();
	const FVector HalfSize = LockOnBox->GetUnscaledBoxExtent();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);

	TArray<FHitResult> BoxHits;

	UKismetSystemLibrary::BoxTraceMulti(this, Start, End, HalfSize, GetControlRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
		EDrawDebugTrace::None, BoxHits, true);

	for (int i = 0; i < BoxHits.Num(); i++)
	{
		if (BoxHits[i].GetActor()->ActorHasTag("Lockable"))
		{
			LockableEnemies.AddUnique(BoxHits[i].GetActor());
		}
	}

}

void APlayerCharacter::UpdateLockOnTarget(float& DeltaTime)
{
	if (bIsLockingOn)
	{

		LockOnBoxTrace();

		if (!LockableEnemies.IsEmpty())
		{
			DetermineFirstLockOnTarget();

			DetermineLeftAndRightTargets();

			LookAtCurrentTarget(DeltaTime);

		}
		else
		{
			bIsLockingOn = false;
		}

	}
	else if (bIsLockingOn == false)
	{
		bIsFirstTimeLockingOn = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void APlayerCharacter::LookAtCurrentTarget(float& DeltaTime)
{
	if (CurrentEnemyTarget)
	{
		LockOnTargetPosition = CurrentEnemyTarget->GetActorLocation();
		const FVector EnemyLocation = UKismetMathLibrary::VLerp(LockOnTargetPosition, GetActorLocation(), DeltaTime);
		const FVector RaisedCameraLocation = Camera->GetComponentLocation() + FVector(0.f, 0.f, CameraHeightLockedOn);
		const FRotator CameraLookAtRotation = UKismetMathLibrary::FindLookAtRotation(RaisedCameraLocation, EnemyLocation);
		const FRotator FaceEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyLocation);
		SetActorRotation(FRotator(GetActorRotation().Pitch, FaceEnemyRotation.Yaw, GetActorRotation().Roll));
		Controller->SetControlRotation(CameraLookAtRotation);
	}
}

void APlayerCharacter::DetermineLeftAndRightTargets()
{
	// If there is more than one enemy. Determines which enemy is the closest on the right or left to lock on to.
	if (LockableEnemies.Num() > 1)
	{
		EnemyTargetRight = LockableEnemies[1];
		EnemyTargetLeft = LockableEnemies[1];
		float LowestAngleOnRight = 360.f;
		float LowestAngleOnLeft = -360.f;

		for (int i = 0; i < LockableEnemies.Num(); i++)
		{
			if (LockableEnemies[i] != CurrentEnemyTarget)
			{
				const FVector EnemyLocation = LockableEnemies[i]->GetActorLocation();
				const FVector EnemyLocationLowered = FVector(EnemyLocation.X, EnemyLocation.Y, GetActorLocation().Z);
				const FVector Forward = GetActorForwardVector();
				const FVector ToEnemy = (EnemyLocationLowered - GetActorLocation()).GetSafeNormal();

				const double CosTheta = FVector::DotProduct(Forward, ToEnemy);
				double Theta = UKismetMathLibrary::Acos(CosTheta);
				Theta = UKismetMathLibrary::RadiansToDegrees(Theta);

				const FVector CrossProduct = FVector::CrossProduct(Forward, ToEnemy);

				if (CrossProduct.Z < 0)
				{
					Theta *= -1.f;
				}

				if (Theta > 0.f)
				{
					if (Theta < LowestAngleOnRight)
					{
						LowestAngleOnRight = Theta;
						EnemyTargetRight = LockableEnemies[i];
					}
				}
				else if (Theta < 0.f)
				{
					if (Theta > LowestAngleOnLeft)
					{
						LowestAngleOnLeft = Theta;
						EnemyTargetLeft = LockableEnemies[i];
					}
				}
			}
		}
	}
}

void APlayerCharacter::DetermineFirstLockOnTarget()
{
	// DOES THIS ONCE Loops through Lockable Enemies and determines the current enemy target by which enemy is closest
	if (bIsFirstTimeLockingOn)
	{
		bIsFirstTimeLockingOn = false;
		CurrentEnemyTarget = LockableEnemies[0];
		LockOnTargetPosition = LockableEnemies[0]->GetActorLocation();

		for (int i = 0; i < LockableEnemies.Num(); i++)
		{
			FVector EnemyLocationElement = LockableEnemies[i]->GetActorLocation();

			if (UKismetMathLibrary::VSizeXY(EnemyLocationElement - GetActorLocation()) < UKismetMathLibrary::VSizeXY(LockOnTargetPosition - GetActorLocation()))
			{
				LockOnTargetPosition = EnemyLocationElement;
				CurrentEnemyTarget = LockableEnemies[i];
			}
		}
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


