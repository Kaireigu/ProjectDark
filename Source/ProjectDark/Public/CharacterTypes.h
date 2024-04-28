#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHanded UMETA(DisplayName = "Equipped One Handed"),
	ECS_EquippedLongbow UMETA(DisplayName = "Equipped Longbow"),
	ECS_EquippedShield UMETA(DisplayName = "Equipped Shield"),
	ECS_EquippedSwordAndShield UMETA(DisplayName = "Equipped Sword and Shield")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Equipping UMETA(DisplayerName = "Equipping"),
	EAS_Dodging UMETA(DisplayName = "Dodging"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Blocking UMETA(DisplayName = "Blocking"),
	EAS_Interacting UMETA(DisplayName = "Interacting"),
	EAS_HitReacting UMETA(DisplayName = "HitReacting"),
	EAS_Climbing UMETA(DisplayName = "Climbing"),
	EAS_Parrying UMETA(DisplayName = "Parrying"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Blocking UMETA(DisplayName = "Blocking"),
	EES_Strafing UMETA(DisplayName = "Strafing"),
	EES_Turning UMETA(DisplayName = "Turning"),
	EES_Unoccupied UMETA(DisplayName = "Unoccupied")
};
