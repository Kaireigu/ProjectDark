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
#include "Potion.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetDefaultControllerValues();

	InitialiseComponents();

	AddActorTags();

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHUD(DeltaTime);
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

void APlayerCharacter::GetHitWithDamage(const float& DamageAmount, const FVector& ImpactPoint)
{

	if (IsNotHitReacting())
	{
		Super::GetHitWithDamage(DamageAmount, ImpactPoint);
		UGameplayStatics::ApplyDamage(this, DamageAmount, nullptr, nullptr, UDamageType::StaticClass());
	}

}


void APlayerCharacter::InteractWithCheckpoint()
{
	if (bCanInteractWithCheckpoint)
	{
		bCanInteractWithCheckpoint = false;
	}
	else
	{
		bCanInteractWithCheckpoint = true;
	}
}

void APlayerCharacter::SetCanGetOnLadder(const bool& CanGetOn, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder)
{
	bCanGetOnLadder = CanGetOn;
	LadderPosition = LadderLocation;
	LadderStartPosition = StartPosition;
	LadderFacingRotation = StartRotation;
	LadderInUse = Ladder;
}

void APlayerCharacter::SetCanGetOffLadder(const bool& CanGetOff, const FVector& LadderLocation, const FVector& StartPosition, const FRotator& StartRotation, IInteractInterface* Ladder)
{
	bCanGetOffLadder = CanGetOff;
	LadderPosition = LadderLocation;
	LadderStartPosition = StartPosition;
	LadderFacingRotation = StartRotation;
	LadderInUse = Ladder;
}

void APlayerCharacter::SetHUDInteractText(const FString& InteractText)
{
	if (HUDOverlay)
	{
		HUDOverlay->SetInteractTextBox(InteractText);
	}
}

void APlayerCharacter::ClearHUDInteractText()
{
	if (HUDOverlay)
	{
		HUDOverlay->ClearInteractTextBox();
	}
}

void APlayerCharacter::SetHUDNotifyText(const FString& NotifyText)
{
	if (HUDOverlay)
	{
		HUDOverlay->SetNotifyTextBox(NotifyText);
		bCanShowNotifyText = true;
	}
}

void APlayerCharacter::ClearHUDNotifyText()
{
	if (HUDOverlay)
	{
		HUDOverlay->ClearNotifyTextBox();
		bCanShowNotifyText = false;
		GetWorldTimerManager().ClearTimer(ClearNotifyTimerHandle);
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ReceiveDamage(DamageAmount);

	if (AttributeComponent == nullptr) { return DamageAmount; }

	if (AttributeComponent->GetHealthPercent() <= 0.f)
	{
		Die();
	}

	return DamageAmount;
}

void APlayerCharacter::PlayHitReactMontage(const FVector& ImpactPoint)
{
	Super::PlayHitReactMontage(ImpactPoint);

	ActionState = EActionState::EAS_HitReacting;
}

void APlayerCharacter::PlayLargeHitReactMontage()
{
	Super::PlayLargeHitReactMontage();

	ActionState = EActionState::EAS_HitReacting;
}

void APlayerCharacter::SetupBossBar(const FString& BossName, const float& BossMaxHealth)
{
	if (HUDOverlay)
	{
		HUDOverlay->SetBossTextBox(BossName);
		CurrentBossMaxHealth = BossMaxHealth;
	}
}

void APlayerCharacter::HideBossBar()
{
	if (HUDOverlay)
	{
		HUDOverlay->HideBossBar();
	}
}

void APlayerCharacter::ShowBossBar()
{
	if (HUDOverlay)
	{
		HUDOverlay->ShowBossBar();
	}
}

void APlayerCharacter::UpdateBossBar(const float& BossCurrentHealth)
{
	if (HUDOverlay)
	{
		HUDOverlay->SetBossHealthPercent(BossCurrentHealth, CurrentBossMaxHealth);
	}
}

void APlayerCharacter::SetDialogueText(const FString& TextToDisplay)
{
	if (HUDOverlay)
	{
		HUDOverlay->SetDialogueTextBox(TextToDisplay);
	}
}

void APlayerCharacter::ClearDialogueText()
{
	if (HUDOverlay)
	{
		HUDOverlay->HideDialogueTextBox();
	}
}

void APlayerCharacter::SetHitBossHeadLandPosition(const FVector& HitPosition)
{
	SetActorLocation(HitPosition);
}

void APlayerCharacter::SetCanOpenDoor(const bool& CanOpenDoor, IInteractInterface* Door)
{
	bCanOpenDoor = CanOpenDoor;

	if (bCanOpenDoor)
	{
		CurrentDoor = Door;
	}
	else
	{
		CurrentDoor = nullptr;
	}
}

void APlayerCharacter::StopInteractionWithCheckpoint()
{
	bCanInteractWithCheckpoint = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialiseSubsystem();

	BindDelegateFunctions();

	SetupHUD();

	SpawnLocation = GetActorTransform();
}

void APlayerCharacter::SetCanCombo(bool CanCombo)
{
	bCanCombo = CanCombo;
}

void APlayerCharacter::AttackEnd()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	bCanUseDodgeBackAttack = false;
	RemoveBackStabTag();

	Tags.Remove(FName("FallAttackActive"));

	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = CameraBoomTargetLength;
	}

	if (bComboActive)
	{
		bComboActive = false;

	}
	else
	{
		SetUnoccupied();
		AnimInstance->StopAllMontages(0.25f);
	}
}

void APlayerCharacter::SetUnoccupied()
{
	ActionState = EActionState::EAS_Unoccupied;
	RemoveParryTag();
	Tags.Remove(FName("Blocking"));
	bCanUseDodgeBackAttack = false;
	StartStaminaRecharge();

	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollision(false);
	}
	
	if (EquippedSecondaryWeapon)
	{
		EquippedSecondaryWeapon->SetWeaponCollision(false);
	}
}

void APlayerCharacter::SetWeaponSocketOnEquipping()
{
	if (EquippedWeapon == nullptr) { return; }

	if (IsUnequipped() || IsShieldEquipped())
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		if (IsShieldEquipped())
		{
			CharacterState = ECharacterState::ECS_EquippedSwordAndShield;
		}
		else if (IsUnequipped())
		{
			CharacterState = ECharacterState::ECS_EquippedOneHanded;
		}
	}
	else if (IsEquippedWithOneHandedWeapon() && EquippedSecondaryWeapon == nullptr || IsEquippedSwordAndShield() && EquippedSecondaryWeapon == nullptr)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));

		if (IsEquippedSwordAndShield())
		{
			CharacterState = ECharacterState::ECS_EquippedShield;
		}
		else if (IsEquippedWithOneHandedWeapon())
		{
			CharacterState = ECharacterState::ECS_Unequipped;
		}
	}
	else if (EquippedSecondaryWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
		EquippedSecondaryWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));

		AWeapon* MainWeapon = EquippedWeapon;
		AWeapon* SecondaryWeapon = EquippedSecondaryWeapon;

		EquippedWeapon = SecondaryWeapon;
		EquippedSecondaryWeapon = MainWeapon;
	}
}

void APlayerCharacter::SetShieldSocketOnEquipping()
{
	if (EquippedShield == nullptr) { return; }

	if (IsUnequipped() || IsEquippedWithOneHandedWeapon())
	{
		EquippedShield->AttachMeshToSocket(GetMesh(), FName("LeftForeArmSocket"));

		if (IsEquippedWithOneHandedWeapon())
		{
			CharacterState = ECharacterState::ECS_EquippedSwordAndShield;
		}
		else if (IsUnequipped())
		{
			CharacterState = ECharacterState::ECS_EquippedShield;
		}
	}
	else if (IsShieldEquipped() || IsEquippedSwordAndShield())
	{
		EquippedShield->AttachMeshToSocket(GetMesh(), FName("SpineSocket2"));

		if (IsEquippedSwordAndShield())
		{
			CharacterState = ECharacterState::ECS_EquippedOneHanded;
		}
		else if (IsShieldEquipped())
		{
			CharacterState = ECharacterState::ECS_Unequipped;
		}
	}
}

void APlayerCharacter::DrinkPotion()
{
	if (EquippedPotion)
	{
		if (EquippedPotion->CanUsePotion())
		{
			ReceiveHealth(EquippedPotion->GetHealAmount());

			EquippedPotion->ReduceUsesByOne();
			HUDOverlay->SetItemUsesTextBox(FString::FromInt(EquippedPotion->GetNumOfUses()));
		}
	}
}

void APlayerCharacter::AttachPotionToHip()
{
	SetUnoccupied();

	if (EquippedPotion)
	{
		EquippedPotion->AttachMeshToSocket(GetMesh(), FName("HipSocket"));
	}
}

void APlayerCharacter::TurnClimbingOff()
{
	SetUnoccupied();
	bShouldDoIKTrace = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->SetPlaneConstraintEnabled(false);

	SetWeaponSocketOnEquipping();
	SetShieldSocketOnEquipping();

	if (LadderInUse)
	{
		LadderInUse->SetLadderNotInUse();
	}
}

void APlayerCharacter::StopMovement()
{
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void APlayerCharacter::AddParryTag()
{
	Tags.AddUnique(FName("Parrying"));
}

void APlayerCharacter::RemoveParryTag()
{
	Tags.Remove(FName("Parrying"));
}

void APlayerCharacter::KickEnemy()
{
	if (CurrentEnemyTargetHitInterface)
	{
		CurrentEnemyTargetHitInterface->InterfacePlayHitReact(GetActorLocation());
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

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	SetRootComponent(GetCapsuleComponent());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = CameraBoomTargetLength;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

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

		if (UseItemAction)
		{
			EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseItem);
		}

		if (SelectRightHandAction)
		{
			EnhancedInputComponent->BindAction(SelectRightHandAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchRightHand);
		}

		if (SelectLeftHandAction)
		{
			EnhancedInputComponent->BindAction(SelectLeftHandAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchLeftHand);
		}

		if (HeavyAttackAction)
		{
			EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HeavyAttack);
		}

		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		}

		if (StopSprintAction)
		{
			EnhancedInputComponent->BindAction(StopSprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StopSprint);
		}

		if (TapR1Action)
		{
			EnhancedInputComponent->BindAction(TapR1Action, ETriggerEvent::Triggered, this, &APlayerCharacter::TapR1);
		}

		if (TapR2Action)
		{
			EnhancedInputComponent->BindAction(TapR2Action, ETriggerEvent::Triggered, this, &APlayerCharacter::TapR2);
		}

		if (PressedL2Action)
		{
			EnhancedInputComponent->BindAction(PressedL2Action, ETriggerEvent::Triggered, this, &APlayerCharacter::PressedL2);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (ActionState == EActionState::EAS_Climbing && GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() == false)
	{
		const FVector2D Movement = value.Get<FVector2D>();

		if (Controller)
		{
			
			if (Movement.X > 0.f)
			{
				PlayMontage(ClimbMontage, FName("Climb"));
			}
			else if (Movement.X < 0.f)
			{
				PlayMontage(ClimbMontage, FName("ClimbDown"));

				if (bCanGetOnLadder)
				{
					TurnClimbingOff();
				}
			}
		}

		if (bCanGetOffLadder && Movement.X > 0.f)
		{
			PlayMontage(ClimbMontage, FName("EndClimb"));
		}
	}

	if (IsOccupied() && ActionState != EActionState::EAS_Blocking) { return; }

	const FVector2D Movement = value.Get<FVector2D>();

	if (IsNotMoving() && Movement.X > 0.f && Movement.Y < 0.15f && Movement.Y > -0.15f)
	{
		bCanBackStabOrKickOrJumpAttack = true;
		GetWorldTimerManager().SetTimer(CannotBackstabTimerHandle, this, &APlayerCharacter::SetCannotBackstabOrKickOrJumpAttack, TimeBufferToBackStab, false);
	}

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

	PickUpWeapon();

	PickUpShield();

	PickUpPotion();

	CheckCanSitAtCheckpoint();

	if (bCanGetOnLadder || bCanGetOffLadder)
	{
		TurnClimbingOn();
	}

	if (bCanOpenDoor && CurrentDoor)
	{
		CurrentDoor->OpenDoor();
	}

	if (bCanShowNotifyText && HUDOverlay)
	{
		bCanShowNotifyText = false;
		HUDOverlay->ShowNotifyTextBox();
		HUDOverlay->ClearInteractTextBox();
		GetWorldTimerManager().SetTimer(ClearNotifyTimerHandle, this, &APlayerCharacter::ClearHUDNotifyText, 3.f, false);
	}

}

void APlayerCharacter::Attack(const FInputActionValue& value)
{
	if (IsUnequipped() || IsShieldEquipped() || GetStamina() < LightAttackStaminaCost || ActionState == EActionState::EAS_HitReacting) { return; }

	bool bIsPlayerFalling = GetCharacterMovement()->IsFalling();

	if (bIsPlayerFalling)
	{
		PlayMontage(AttackMontageOneHanded, FName("FallAttack"));
		Tags.AddUnique(FName("FallAttackActive"));
		return;
	}

	if (bCanUseDodgeBackAttack)
	{
		bCanUseDodgeBackAttack = false;
		PlayMontage(AttackMontageOneHanded, FName("DodgeBackAttack"));
		UseStamina(LightAttackStaminaCost);
		ActionState = EActionState::EAS_Attacking;
		return;
	}

	if (bCanCombo)
	{
		bComboActive = true;
	}

	if (IsOccupied() || bCanBackStabOrKickOrJumpAttack) { return; }

	PlayMontage(AttackMontageOneHanded, FName("Attack1"));
	UseStamina(LightAttackStaminaCost);
	ActionState = EActionState::EAS_Attacking;
}

void APlayerCharacter::HeavyAttack(const FInputActionValue& value)
{
	if (IsUnequipped() || IsShieldEquipped() || GetStamina() < HeavyAttackStaminaCost || ActionState == EActionState::EAS_HitReacting) { return; }

	if (bCanCombo)
	{
		bComboActive = true;
	}

	if (IsOccupied() || bCanBackStabOrKickOrJumpAttack) { return; }

	PlayMontage(AttackMontageOneHanded, FName("HeavyAttack"));
	UseStamina(HeavyAttackStaminaCost);
	ActionState = EActionState::EAS_Attacking;
}

void APlayerCharacter::RollOrBackStep(const FInputActionValue& value)
{
	if (IsOccupied()) { return; }

	if (IsMoving() && GetStamina() >= RollStaminaCost)
	{
		FVector RollDirection = GetVelocity().GetSafeNormal();
		FRotator NewRotation = RollDirection.Rotation();
		SetActorRotation(NewRotation);
		PlayMontage(RollMontage, FName("Roll1"));
		UseStamina(RollStaminaCost);
		CreateFields(GetActorLocation());
		ActionState = EActionState::EAS_Dodging;

	}
	else if (IsNotMoving() && GetStamina() >= BackstepStaminaCost)
	{
		PlayMontage(BackStepMontage, FName("Default"));
		UseStamina(BackstepStaminaCost);
		bCanUseDodgeBackAttack = true;
		ActionState = EActionState::EAS_Dodging;
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
	if (IsOccupied() && ActionState != EActionState::EAS_Blocking) { return; }

	const float Input = value.Get<float>();

	if (Input > 0.f && EnemyTargetRight)
	{
		CurrentEnemyTargetHitInterface = Cast<IHitInterface>(CurrentEnemyTarget);

		if (CurrentEnemyTargetHitInterface)
		{
			CurrentEnemyTargetHitInterface->BeLockedOff();
		}

		CurrentEnemyTarget = EnemyTargetRight;

		CurrentEnemyTargetHitInterface = Cast<IHitInterface>(CurrentEnemyTarget);

		if (CurrentEnemyTargetHitInterface)
		{
			CurrentEnemyTargetHitInterface->BeLockedOnTo();
		}

		IsDeadEnemy = Cast<AEnemy>(CurrentEnemyTarget);
		IsDeadEnemy->EnemyDied.AddUniqueDynamic(this, &APlayerCharacter::OnEnemyDeath);
	}
	else if (Input < 0.f && EnemyTargetLeft)
	{
		CurrentEnemyTargetHitInterface = Cast<IHitInterface>(CurrentEnemyTarget);

		if (CurrentEnemyTargetHitInterface)
		{
			CurrentEnemyTargetHitInterface->BeLockedOff();
		}
		CurrentEnemyTarget = EnemyTargetLeft;

		CurrentEnemyTargetHitInterface = Cast<IHitInterface>(CurrentEnemyTarget);

		if (CurrentEnemyTargetHitInterface)
		{
			CurrentEnemyTargetHitInterface->BeLockedOnTo();
		}

		IsDeadEnemy = Cast<AEnemy>(CurrentEnemyTarget);
		IsDeadEnemy->EnemyDied.AddUniqueDynamic(this, &APlayerCharacter::OnEnemyDeath);
	}
}

void APlayerCharacter::Block(const FInputActionValue& value)
{
	if (IsOccupied() || EquippedShield == nullptr || IsShieldEquipped() == false && IsEquippedSwordAndShield() == false || ActionState == EActionState::EAS_HitReacting) { return; }

	PlayMontage(BlockMontage, FName("BlockingIdle"));
	ActionState = EActionState::EAS_Blocking;
	Tags.AddUnique("Blocking");
}

void APlayerCharacter::StopBlock(const FInputActionValue& value)
{
	if (ActionState == EActionState::EAS_HitReacting) { return; }

	if (ActionState == EActionState::EAS_Blocking && EquippedShield)
	{
		ActionState = EActionState::EAS_Unoccupied;
		StopAllMontages();
		Tags.Remove("Blocking");
		RechargeStamina();
	}

}

void APlayerCharacter::UseItem(const FInputActionValue& value)
{
	if (EquippedPotion && EquippedPotion->CanUsePotion())
	{
		EquippedPotion->AttachMeshToSocket(GetMesh(), FName("LeftHandSocket"));
		PlayMontage(PotionMontage, FName("Default"));
		ActionState = EActionState::EAS_Equipping;
	}
}

void APlayerCharacter::SwitchRightHand(const FInputActionValue& value)
{
	SetWeaponSocketOnEquipping();
}

void APlayerCharacter::SwitchLeftHand(const FInputActionValue& value)
{
	SetShieldSocketOnEquipping();
}

void APlayerCharacter::Sprint(const FInputActionValue& value)
{
	if (IsMoving() && GetIsLockedOn() == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		StopStaminaRecharge();
		StartDrainStamina();
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = Walkspeed;
	StopDrainStamina();
	StartStaminaRecharge();
}

void APlayerCharacter::TapR1(const FInputActionValue& value)
{
	if (IsUnequipped() || IsShieldEquipped() || GetStamina() < LightAttackStaminaCost || ActionState == EActionState::EAS_HitReacting || IsOccupied()) { return; }


	if (bCanBackStabOrKickOrJumpAttack)
	{
		if (CurrentEnemyTarget && IsFacing(CurrentEnemyTarget) && InTargetRange(CurrentEnemyTarget, BackStabAttackRange) && CurrentEnemyTarget->ActorHasTag("Staggered"))
		{
			CameraBoom->TargetArmLength = BackStabCameraBoomTargetLength;
			PlayMontage(AttackMontageOneHanded, FName("BackStab"));
			IHitInterface* CurrenyEnemyHitInterface = Cast<IHitInterface>(CurrentEnemyTarget);

			if (CurrenyEnemyHitInterface && EquippedWeapon)
			{
				CurrentEnemyTargetHitInterface->PlayBackStab();
				UGameplayStatics::ApplyDamage(CurrentEnemyTarget, EquippedWeapon->GetBackStabDamage(), GetController(), this, UDamageType::StaticClass());
			}


			ActionState = EActionState::EAS_Attacking;
			return;
		}
		else if (CurrentEnemyTarget && EnemyIsFacingMe(CurrentEnemyTarget) == false && InTargetRange(CurrentEnemyTarget, BackStabAttackRange) && CurrentEnemyTarget->ActorHasTag("Boss") == false)
		{
			CameraBoom->TargetArmLength = BackStabCameraBoomTargetLength;
			PlayMontage(AttackMontageOneHanded, FName("BackStab"));
			IHitInterface* CurrenyEnemyHitInterface = Cast<IHitInterface>(CurrentEnemyTarget);

			if (CurrenyEnemyHitInterface && EquippedWeapon)
			{
				CurrentEnemyTargetHitInterface->PlayBackStab();
				UGameplayStatics::ApplyDamage(CurrentEnemyTarget, EquippedWeapon->GetBackStabDamage(), GetController(), this, UDamageType::StaticClass());
			}


			ActionState = EActionState::EAS_Attacking;
			return;
		}
		else
		{
			PlayMontage(AttackMontageOneHanded, FName("Kick"));
			ActionState = EActionState::EAS_Attacking;
		}

	}
}

void APlayerCharacter::TapR2(const FInputActionValue& value)
{
	if (IsUnequipped() || IsShieldEquipped() || GetStamina() < LightAttackStaminaCost || ActionState == EActionState::EAS_HitReacting || IsOccupied()) { return; }

	if (bCanBackStabOrKickOrJumpAttack)
	{
		PlayMontage(AttackMontageOneHanded, FName("JumpAttack"));
		ActionState = EActionState::EAS_Attacking;
	}
}

void APlayerCharacter::PressedL2(const FInputActionValue& value)
{
	if (IsOccupied() || EquippedShield == nullptr || IsShieldEquipped() == false && IsEquippedSwordAndShield() == false) { return; }

	PlayMontage(BlockMontage, FName("Parry"));
	ActionState = EActionState::EAS_Parrying;
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

		if (CurrentEnemyTargetHitInterface)
		{
			CurrentEnemyTargetHitInterface->BeLockedOff();
			CurrentEnemyTargetHitInterface = nullptr;
		}
	}
}

void APlayerCharacter::LookAtCurrentTarget(float& DeltaTime)
{
	if (CurrentEnemyTarget)
	{
		LockOnTargetPosition = CurrentEnemyTarget->GetActorLocation();
		const FVector EnemyLocation = UKismetMathLibrary::VLerp(LockOnTargetPosition, GetActorLocation(), DeltaTime);
		const FVector RaisedEnemyLocation = FVector(EnemyLocation.X, EnemyLocation.Y, EnemyLocation.Z + 10.f);
		const FVector RaisedCameraLocation = GetActorLocation() + FVector(0.f, 0.f, CameraHeightLockedOn);
		const FRotator CameraLookAtRotation = UKismetMathLibrary::FindLookAtRotation(RaisedCameraLocation, RaisedEnemyLocation);
		if (ActionState != EActionState::EAS_Dodging)
		{
			const FRotator FaceEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyLocation);
			SetActorRotation(FRotator(GetActorRotation().Pitch, UKismetMathLibrary::Lerp(FaceEnemyRotation.Yaw, GetActorRotation().Yaw, DeltaTime), GetActorRotation().Roll));
		}

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
	if (bIsFirstTimeLockingOn == false || Camera == nullptr || LockableEnemies.Num() == 0) { return; }

	bIsFirstTimeLockingOn = false;
	CurrentEnemyTarget = LockableEnemies[0];
	LockOnTargetPosition = LockableEnemies[0]->GetActorLocation();
	ToEnemyAngle = GetTheta(Camera->GetForwardVector(), LockableEnemies[0]->GetActorLocation());

	if (ToEnemyAngle < 0.f)
	{
		ToEnemyAngle *= -1.f;
	}

	for (int i = 0; i < LockableEnemies.Num(); i++)
	{
		FVector EnemyLocationElement = LockableEnemies[i]->GetActorLocation();
		double ToEnemyAngleElement = GetTheta(Camera->GetForwardVector(), LockableEnemies[i]->GetActorLocation());

		if (ToEnemyAngleElement < 0.f)
		{
			ToEnemyAngleElement *= -1.f;
		}

		if (ToEnemyAngleElement < ToEnemyAngle)
		{
			ToEnemyAngle = ToEnemyAngleElement;
			LockOnTargetPosition = LockableEnemies[i]->GetActorLocation();
			CurrentEnemyTarget = LockableEnemies[i];
		}
	}

	CurrentEnemyTargetHitInterface = Cast<IHitInterface>(CurrentEnemyTarget);

	if (CurrentEnemyTargetHitInterface)
	{
		CurrentEnemyTargetHitInterface->BeLockedOnTo();
	}

	IsDeadEnemy = Cast<AEnemy>(CurrentEnemyTarget);

	if (IsDeadEnemy)
	{
		IsDeadEnemy->EnemyDied.AddUniqueDynamic(this, &APlayerCharacter::OnEnemyDeath);
	}
}

void APlayerCharacter::CheckLockOnTargetDistance()
{
	if (CurrentEnemyTarget && UKismetMathLibrary::VSizeXY(CurrentEnemyTarget->GetActorLocation() - GetActorLocation()) > LockOffDistance)
	{
		SetLockOffValues();
	}

	if (EnemyTargetLeft)
	{
		double ToEnemyLeft = UKismetMathLibrary::VSizeXY(EnemyTargetLeft->GetActorLocation() - GetActorLocation());
		double AngleBetweenThisAndEnemyLeft = GetTheta(GetActorForwardVector(), EnemyTargetLeft->GetActorLocation());

		if (ToEnemyLeft > 500.f && AngleBetweenThisAndEnemyLeft > 135.f
			|| ToEnemyLeft > 500.f && AngleBetweenThisAndEnemyLeft < -135.f)
		{
			LockableEnemies.Remove(EnemyTargetLeft);
			EnemyTargetLeft = nullptr;
		}
	}

	if (EnemyTargetRight)
	{
		double ToEnemyRight = UKismetMathLibrary::VSizeXY(EnemyTargetRight->GetActorLocation() - GetActorLocation());
		double AngleBetweenThisAndEnemyRight = GetTheta(GetActorForwardVector(), EnemyTargetRight->GetActorLocation());

		if (ToEnemyRight > 500.f && AngleBetweenThisAndEnemyRight > 135.f
			|| ToEnemyRight > 500.f && AngleBetweenThisAndEnemyRight < -135.f)
		{
			LockableEnemies.Remove(EnemyTargetRight);
			EnemyTargetRight = nullptr;
		}
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
		CurrentEnemyTargetHitInterface = nullptr;
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

bool APlayerCharacter::IsEquippedSwordAndShield()
{
	return CharacterState == ECharacterState::ECS_EquippedSwordAndShield;
}

bool APlayerCharacter::IsShieldEquipped()
{
	return CharacterState == ECharacterState::ECS_EquippedShield;
}

bool APlayerCharacter::IsNotHitReacting()
{
	return ActionState != EActionState::EAS_HitReacting;
}

void APlayerCharacter::ReceiveHealth(const float& HealAmount)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveHeal(HealAmount);

		if (HUDOverlay)
		{
			HUDOverlay->SetHealthBarPercent(AttributeComponent->GetHealthPercent(), AttributeComponent->GetMaxHealth());
		}
	}
}

void APlayerCharacter::ReceiveDamage(const float& DamageAmount)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		if (HUDOverlay)
		{
			HUDOverlay->SetHealthBarPercent(AttributeComponent->GetHealthPercent(), AttributeComponent->GetMaxHealth());
		}
	}
}

void APlayerCharacter::StartStaminaRecharge()
{
	if (AttributeComponent)
	{
		AttributeComponent->StartStaminaRecharge();
	}
}

void APlayerCharacter::StopStaminaRecharge()
{
	if (AttributeComponent)
	{
		AttributeComponent->StopStaminaRecharge();
	}
}

void APlayerCharacter::StartDrainStamina()
{
	if (AttributeComponent)
	{
		AttributeComponent->StartDrainStamina();
	}
}

void APlayerCharacter::StopDrainStamina()
{
	if (AttributeComponent)
	{
		AttributeComponent->StopDrainStamina();
		StartStaminaRecharge();
	}
}

float APlayerCharacter::GetStamina()
{
	if (AttributeComponent)
	{
		return AttributeComponent->GetCurrentStamina();
	}
	else
	{
		return 0.f;
	}
}

void APlayerCharacter::UseStamina(const float& StaminaAmount)
{
	if (AttributeComponent && HUDOverlay)
	{
		AttributeComponent->UseStamina(StaminaAmount);
		AttributeComponent->StopStaminaRecharge();
		HUDOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent(), AttributeComponent->GetMaxStamina());
	}
}

void APlayerCharacter::RechargeStamina()
{
	StartStaminaRecharge();
}

void APlayerCharacter::PlayHitShieldSound()
{
	Super::PlayHitShieldSound();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (PlayerController == nullptr) { return; }

	PlayerController->PlayDynamicForceFeedback(100.f, 0.25f, true, false, false, true);
}

void APlayerCharacter::Die()
{
	Super::Die();

	Respawn();
}

void APlayerCharacter::AddActorTags()
{
	Tags.AddUnique(FName("Hitable"));
	Tags.AddUnique(FName("PlayerCharacter"));
}

void APlayerCharacter::SetupHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		AProjectDarkHUD* ProjectDarkHUD = Cast<AProjectDarkHUD>(PlayerController->GetHUD());

		if (ProjectDarkHUD)
		{
			HUDOverlay = ProjectDarkHUD->GetHUDOverlay();

			if (HUDOverlay && AttributeComponent)
			{
				AttributeComponent->SetInitialHealthAndStaminaValues();
				HUDOverlay->SetHealthBarPercent(AttributeComponent->GetHealthPercent(), AttributeComponent->GetMaxHealth());
				HUDOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent(), AttributeComponent->GetMaxStamina());
				HUDOverlay->HideInteractTextBox();
				HUDOverlay->HideDialogueTextBox();
				HUDOverlay->HideBossBar();
				HUDOverlay->HideNotifyTextBox();
				HUDOverlay->HideItemUsesTextBox();
			}
		}
	}
}

void APlayerCharacter::UpdateHUD(const float& DeltaTime)
{
	if (HUDOverlay && AttributeComponent)
	{
		AttributeComponent->RechargeStamina(DeltaTime);
		HUDOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent(), AttributeComponent->GetMaxStamina());
	}
	
	if (HUDOverlay && AttributeComponent)
	{
		AttributeComponent->DrainStamina(DeltaTime);
		HUDOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent(), AttributeComponent->GetMaxStamina());
	}
}

void APlayerCharacter::BindDelegateFunctions()
{
	LockOnBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnLockBoxBeginOverlap);
	LockOnBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnLockBoxEndOverlap);
}

void APlayerCharacter::PickUpWeapon()
{
	if (EquippedWeapon == nullptr && OverlappingItem)
	{
		if (OverlappingItem->ActorHasTag("Weapon"))
		{
			EquippedWeapon = Cast<AWeapon>(OverlappingItem);

			if (EquippedWeapon && EquippedWeapon->IsItemUnequipped())
			{
				EquippedWeapon->SetOwner(this);
				EquippedWeapon->SetInstigator(this);
				EquippedWeapon->SetItemStateEquipped();

				if (IsUnequipped())
				{
					CharacterState = ECharacterState::ECS_EquippedOneHanded;
					EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
				}
				else if (IsShieldEquipped())
				{
					CharacterState = ECharacterState::ECS_EquippedSwordAndShield;
					EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
				}
				else if (IsEquippedWithOneHandedWeapon() || IsEquippedSwordAndShield())
				{
					EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
				}

			}
			else
			{
				EquippedWeapon = nullptr;
			}
		}

	}
	else if (EquippedSecondaryWeapon == nullptr && OverlappingItem)
	{
		if (OverlappingItem->ActorHasTag("Weapon"))
		{
			EquippedSecondaryWeapon = Cast<AWeapon>(OverlappingItem);

			if (EquippedSecondaryWeapon && EquippedSecondaryWeapon->IsItemUnequipped())
			{
				EquippedSecondaryWeapon->SetOwner(this);
				EquippedSecondaryWeapon->SetInstigator(this);
				EquippedSecondaryWeapon->SetItemStateEquipped();

				if (IsUnequipped())
				{
					CharacterState = ECharacterState::ECS_EquippedOneHanded;
					EquippedSecondaryWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
				}
				else if (IsShieldEquipped())
				{
					EquippedSecondaryWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
					CharacterState = ECharacterState::ECS_EquippedSwordAndShield;
				}
				else if (IsEquippedWithOneHandedWeapon() || IsEquippedSwordAndShield())
				{
					EquippedSecondaryWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
				}

			}
			else
			{
				EquippedSecondaryWeapon = nullptr;
			}
		}
	}
}

void APlayerCharacter::PickUpShield()
{
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

				if (IsUnequipped())
				{
					CharacterState = ECharacterState::ECS_EquippedShield;
				}
				else if (IsEquippedWithOneHandedWeapon())
				{
					CharacterState = ECharacterState::ECS_EquippedSwordAndShield;
				}
			}
			else
			{
				EquippedShield = nullptr;
			}
		}
	}
}

void APlayerCharacter::PickUpPotion()
{
	if (EquippedPotion == nullptr && OverlappingItem)
	{
		if (OverlappingItem->ActorHasTag("Potion"))
		{
			EquippedPotion = Cast<APotion>(OverlappingItem);

			if (EquippedPotion && EquippedPotion->IsItemUnequipped())
			{
				EquippedPotion->AttachMeshToSocket(GetMesh(), FName("HipSocket"));
				EquippedPotion->SetOwner(this);
				EquippedPotion->SetItemStateEquipped();

				if (HUDOverlay == nullptr) { return; }

				HUDOverlay->SetItemUsesTextBox(FString::FromInt(EquippedPotion->GetNumOfUses()));
				HUDOverlay->ShowItemUsesTextBox();
			}
			else
			{
				EquippedPotion = nullptr;
			}
		}
	}
}

void APlayerCharacter::CheckCanSitAtCheckpoint()
{
	if (bCanInteractWithCheckpoint && ActionState != EActionState::EAS_Interacting)
	{
		ActionState = EActionState::EAS_Interacting;
		PlayMontage(SitMontage, FName("Default"));
		ClearHUDInteractText();
		SpawnLocation = GetActorTransform();

		if (AttributeComponent)
		{
			ReceiveHealth(AttributeComponent->GetMaxHealth());

			if (EquippedPotion)
			{
				EquippedPotion->ResetNumberOfUses();

				if (HUDOverlay == nullptr) { return; }

				HUDOverlay->SetItemUsesTextBox(FString::FromInt(EquippedPotion->GetNumOfUses()));
				HUDOverlay->ShowItemUsesTextBox();
			}
		}

		if (IsEquippedSwordAndShield())
		{
			SetWeaponSocketOnEquipping();
			SetShieldSocketOnEquipping();
		}
		else if (IsEquippedWithOneHandedWeapon())
		{
			SetWeaponSocketOnEquipping();
		}
		else if (IsShieldEquipped())
		{
			SetShieldSocketOnEquipping();
		}
	}
	else if (bCanInteractWithCheckpoint && ActionState == EActionState::EAS_Interacting)
	{
		PlayMontage(SitMontage, FName("StandingUp"));
		SetWeaponSocketOnEquipping();
		SetShieldSocketOnEquipping();

	}
}

void APlayerCharacter::TurnClimbingOn()
{
	SetActorLocation(LadderStartPosition);
	SetActorRotation(LadderFacingRotation);
	Controller->SetControlRotation(LadderFacingRotation);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
	PlayMontage(ClimbMontage, FName("Climb"));
	ActionState = EActionState::EAS_Climbing;
	bShouldDoIKTrace = false;
	
	SetWeaponSocketOnEquipping();
	SetShieldSocketOnEquipping();

	if (LadderInUse)
	{
		LadderInUse->SetLadderInUse();
	}
}

void APlayerCharacter::SetCannotBackstabOrKickOrJumpAttack()
{
	bCanBackStabOrKickOrJumpAttack = false;
}

void APlayerCharacter::Respawn()
{
	if (AttributeComponent == nullptr) { return; }
	
	ReceiveHealth(AttributeComponent->GetMaxHealth());
	SetActorTransform(SpawnLocation);
	PlayMontage(SitMontage, FName("StandingUp"));

	if (EquippedPotion == nullptr) { return; }

	EquippedPotion->ResetNumberOfUses();

	if (HUDOverlay == nullptr) { return; }

	HUDOverlay->SetItemUsesTextBox(FString::FromInt(EquippedPotion->GetNumOfUses()));
	HUDOverlay->ShowItemUsesTextBox();
}

double APlayerCharacter::GetMovementX()
{
	return GetCharacterMovement()->Velocity.X;
}

double APlayerCharacter::GetMovementY()
{
	return GetCharacterMovement()->Velocity.Y;
}
