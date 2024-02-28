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
#include "Enemy.h"
#include "HUDOverlay.h"
#include "ProjectDarkHUD.h"
#include "Attributes.h"
#include "Shield.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetDefaultControllerValues();

	InitialiseComponents();

	Tags.AddUnique(FName("Hitable"));
	Tags.AddUnique(FName("PlayerCharacter"));

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

void APlayerCharacter::GetHit(AActor* OtherActor, const FVector& ImpactPoint)
{
	
	Super::GetHit(OtherActor, ImpactPoint);

}

float APlayerCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		if (HUDOverlay)
		{
			HUDOverlay->SetHealthBarPercent(AttributeComponent->GetHealthPercent(), AttributeComponent->GetMaxHealth());
		}
	}

	return DamageAmount;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialiseSubsystem();

	LockOnBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnLockBoxBeginOverlap);
	LockOnBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnLockBoxEndOverlap);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		AProjectDarkHUD* ProjectDarkHUD = Cast<AProjectDarkHUD>(PlayerController->GetHUD());

		if (ProjectDarkHUD)
		{
			HUDOverlay = ProjectDarkHUD->GetHUDOverlay();

			if (HUDOverlay && AttributeComponent)
			{
				HUDOverlay->SetHealthBarPercent(AttributeComponent->GetHealthPercent(), AttributeComponent->GetMaxHealth());
				HUDOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent(), AttributeComponent->GetMaxStamina());
			}
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

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	LockOnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Lock On Box"));
	LockOnBox->SetupAttachment(Camera);
	LockOnBox->ComponentTags.AddUnique(FName("LockOnBox"));

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

		if (BlockAction)
		{
			EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Block);
		}

		if (StopBlockAction)
		{
			EnhancedInputComponent->BindAction(StopBlockAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StopBlock);
		}

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

	if (EquippedWeapon == nullptr && OverlappingItem)
	{
		if (OverlappingItem->ActorHasTag("Weapon"))
		{
			EquippedWeapon = Cast<AWeapon>(OverlappingItem);

			if (EquippedWeapon && EquippedWeapon->IsItemUnequipped())
			{
				EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
				EquippedWeapon->SetOwner(this);
				EquippedWeapon->SetInstigator(this);
				EquippedWeapon->SetItemStateEquipped();
				CharacterState = ECharacterState::ECS_EquippedOneHanded;
			}
			else
			{
				EquippedWeapon = nullptr;
			}
		}

	}

	if (EquippedShield == nullptr && OverlappingItem)
	{
		if (OverlappingItem->ActorHasTag("Shield"))
		{
			EquippedShield = Cast<AShield>(OverlappingItem);

			if (EquippedShield && EquippedShield->IsItemUnequipped())
			{
				EquippedShield->AttachMeshToSocket(GetMesh(), FName("LeftForeArmSocket"));
				EquippedShield->SetOwner(this);
				EquippedShield->SetItemStateEquipped();
			}
			else
			{
				EquippedShield = nullptr;
			}
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
		CreateFields(GetActorLocation());
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
		SetLockOffValues();
	}
	else
	{
		bIsLockingOn = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

}

void APlayerCharacter::SwitchLockOnTarget(const FInputActionValue& value)
{
	if (IsOccupied()) { return; }

	const float Input = value.Get<float>();

	if (Input > 0.f && EnemyTargetRight)
	{
		CurrentEnemyTarget = EnemyTargetRight;
		IsDeadEnemy = Cast<AEnemy>(CurrentEnemyTarget);
		IsDeadEnemy->EnemyDied.AddUniqueDynamic(this, &APlayerCharacter::OnEnemyDeath);
	}
	else if (Input < 0.f && EnemyTargetLeft)
	{
		CurrentEnemyTarget = EnemyTargetLeft;
		IsDeadEnemy = Cast<AEnemy>(CurrentEnemyTarget);
		IsDeadEnemy->EnemyDied.AddUniqueDynamic(this, &APlayerCharacter::OnEnemyDeath);
	}
}

void APlayerCharacter::Block(const FInputActionValue& value)
{
	if (IsOccupied() || EquippedShield == nullptr) { return; }

	PlayMontage(BlockMontage, FName("StartBlock"));
	ActionState = EActionState::EAS_Blocking;
	Tags.AddUnique("Blocking");
}

void APlayerCharacter::StopBlock(const FInputActionValue& value)
{
	if (ActionState != EActionState::EAS_Blocking || EquippedShield == nullptr)

	PlayMontage(BlockMontage, FName("StopBlock"));
	ActionState = EActionState::EAS_Unoccupied;
	Tags.Remove("Blocking");
}

void APlayerCharacter::OnLockBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Lockable"))
	{
		LockableEnemies.AddUnique(OtherActor);
	}
}

void APlayerCharacter::OnLockBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Lockable"))
	{
		LockableEnemies.Remove(OtherActor);
	}
}

void APlayerCharacter::LockOnBoxTrace()
{
	// Gets all the enemies within a trace and stores them in Lockable Enemies
	//LockableEnemies.Empty();

	const FVector Start = StartTraceLocation->GetComponentLocation();
	const FVector End = EndTraceLocation->GetComponentLocation();
	const FVector HalfSize = FVector(LockOnBox->GetUnscaledBoxExtent());

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);

	TArray<FHitResult> BoxHits;

	UWorld* World = GetWorld();
	FCollisionQueryParams QueryParams = FCollisionQueryParams();
	QueryParams.AddIgnoredActor(this);
	FCollisionResponseParams ResponseParams = FCollisionResponseParams();

	UKismetSystemLibrary::BoxTraceMulti(this, Start, End, HalfSize, GetControlRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
		EDrawDebugTrace::None, BoxHits, true);

	// From the BoxHits, do a line trace between the enemy and the player to determine if anything is between them. If there isn't, add the enemy as a lockable target
	for (int i = 0; i < BoxHits.Num(); i++)
	{
		if (BoxHits[i].GetActor()->ActorHasTag("Lockable") && World)
		{
			FHitResult LineTraceHit;
			QueryParams.AddIgnoredActor(BoxHits[i].GetActor());
			World->LineTraceSingleByChannel(LineTraceHit, BoxHits[i].GetActor()->GetActorLocation(), GetActorLocation(), ECollisionChannel::ECC_Visibility, QueryParams, ResponseParams);

			if (!LineTraceHit.IsValidBlockingHit())
			{
				LockableEnemies.AddUnique(BoxHits[i].GetActor());
			}
			else
			{
				LockableEnemies.Remove(BoxHits[i].GetActor());
			}

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
			CheckLockOnTargetDistance();

			if (bIsLockingOn)
			{
				DetermineFirstLockOnTarget();

				DetermineLeftAndRightTargets();

				LookAtCurrentTarget(DeltaTime);
			}

		}
		else
		{
			SetLockOffValues();
		}

	}
	else if (bIsLockingOn == false)
	{
		SetLockOffValues();
	}
}

void APlayerCharacter::LookAtCurrentTarget(float& DeltaTime)
{
	if (CurrentEnemyTarget)
	{
		LockOnTargetPosition = CurrentEnemyTarget->GetActorLocation();
		const FVector EnemyLocation = UKismetMathLibrary::VLerp(LockOnTargetPosition, GetActorLocation(), DeltaTime);
		const FVector RaisedEnemyLocation = FVector(EnemyLocation.X, EnemyLocation.Y, EnemyLocation.Z + 10.f);
		const FVector RaisedCameraLocation = Camera->GetComponentLocation() + FVector(0.f, 0.f, CameraHeightLockedOn);
		const FRotator CameraLookAtRotation = UKismetMathLibrary::FindLookAtRotation(RaisedCameraLocation, RaisedEnemyLocation);
		const FRotator FaceEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyLocation);
		SetActorRotation(FRotator(GetActorRotation().Pitch, UKismetMathLibrary::Lerp(FaceEnemyRotation.Yaw, GetActorRotation().Yaw, DeltaTime), GetActorRotation().Roll));
		Controller->SetControlRotation(UKismetMathLibrary::RLerp(CameraLookAtRotation, GetControlRotation(), DeltaTime, true));
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

				double Theta = GetTheta(GetActorForwardVector(), LockableEnemies[i]->GetActorLocation());

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

		IsDeadEnemy = Cast<AEnemy>(CurrentEnemyTarget);
		
		if (IsDeadEnemy)
		{
			IsDeadEnemy->EnemyDied.AddUniqueDynamic(this, &APlayerCharacter::OnEnemyDeath);
		}
	}
}

void APlayerCharacter::CheckLockOnTargetDistance()
{
	if (CurrentEnemyTarget && UKismetMathLibrary::VSizeXY(CurrentEnemyTarget->GetActorLocation() - GetActorLocation()) > LockOffDistance)
	{
		SetLockOffValues();
	}
	
	if (EnemyTargetLeft && UKismetMathLibrary::VSizeXY(EnemyTargetLeft->GetActorLocation() - GetActorLocation()) > 500.f && GetTheta(GetActorForwardVector(), EnemyTargetLeft->GetActorLocation()) > 135.f
		|| EnemyTargetLeft && UKismetMathLibrary::VSizeXY(EnemyTargetLeft->GetActorLocation() - GetActorLocation()) > 500.f && GetTheta(GetActorForwardVector(), EnemyTargetLeft->GetActorLocation()) < -135.f)
	{
		LockableEnemies.Remove(EnemyTargetLeft);
		EnemyTargetLeft = nullptr;
	}

	if (EnemyTargetRight && UKismetMathLibrary::VSizeXY(EnemyTargetRight->GetActorLocation() - GetActorLocation()) > 500.f && GetTheta(GetActorForwardVector(), EnemyTargetRight->GetActorLocation()) > 135.f
		|| EnemyTargetRight && UKismetMathLibrary::VSizeXY(EnemyTargetRight->GetActorLocation() - GetActorLocation()) > 500.f && GetTheta(GetActorForwardVector(), EnemyTargetRight->GetActorLocation()) < -135.f)
	{
		LockableEnemies.Remove(EnemyTargetRight);
		EnemyTargetRight = nullptr;
	}
}

void APlayerCharacter::SetLockOffValues()
{
	bIsLockingOn = false;
	bIsFirstTimeLockingOn = true;
	CurrentEnemyTarget = nullptr;
	EnemyTargetRight = nullptr;
	EnemyTargetLeft = nullptr;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	LockableEnemies.Empty();

	if (IsDeadEnemy)
	{
		IsDeadEnemy->EnemyDied.RemoveDynamic(this, &APlayerCharacter::OnEnemyDeath);
	}
	IsDeadEnemy = nullptr;
}

void APlayerCharacter::OnEnemyDeath(AActor* Enemy)
{
	if (CurrentEnemyTarget == Enemy)
	{
		SetLockOffValues();
		
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


