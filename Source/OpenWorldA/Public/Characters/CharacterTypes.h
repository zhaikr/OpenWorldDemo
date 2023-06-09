#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
  ECS_Uequipped UMETA(DisplayName = "Uequipped"),
  ECS_EquippedSingleWeapon UMETA(DisplayName = "EquippedSingleWeapon"),
  ECS_EquippedDoubleWeapon UMETA(DisplayName = "EquippedDoubleWeapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
  EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
  EAS_Attacking UMETA(DisplayName = "Attacking"),
  EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
  EDP_Alive UMETA(DisplayName = "Alive"),
  EDP_Death1 UMETA(DisplayName = "Death1"),
  EDP_Death2 UMETA(DisplayName = "Death2"),
  EDP_Death3 UMETA(DisplayName = "Death3"),
  EDP_Death4 UMETA(DisplayName = "Death4"),
  EDP_Death5 UMETA(DisplayName = "Death5"),
  EDP_Death6 UMETA(DisplayName = "Death6")
};  

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
  EES_Dead UMETA(DisplayName = "Dead"),
  EES_Patrolling UMETA(DisplayName = "Patrolling"),
  EES_Chasing UMETA(DisplayName = "Chasing"),
  EES_Attacking UMETA(DisplayName = "Attacking"),
  EES_Engaged UMETA(DisplayName = "Engaged")
};