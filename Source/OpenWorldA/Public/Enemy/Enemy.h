 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class OPENWORLDA_API AEnemy : public ABaseCharacter
{
   GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  //random Get target from PatrolTargets
  void CheckPatroltarget();

  void CheckCombatTarget();

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

  //inherit from AActor
  virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

  virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  /**
  * Play Montage Function
  */
  virtual void Attack() override;

  virtual void PlayAttackMontage() override;

  virtual bool CanAttack() override;

  virtual void HandleDamage(float DamageAmount) override;

  virtual void Die() override;

  UPROPERTY(BlueprintReadOnly)
  EDeathPose DeathPose = EDeathPose::EDP_Alive;

  UPROPERTY(BlueprintReadOnly)
  EEnemyState EnemyState = EEnemyState::EES_Patrolling;

  bool InTargetRange(AActor* Target, double Radius);
  void MoveToTarget(AActor* Target);
  AActor* ChoosePatrolTarget();

  UFUNCTION()
  void PawnSeen(APawn* SeenPawn);



private:
  /*
  *  Components
  */

  UPROPERTY(VisibleAnywhere)
  UHealthBarComponent* HealthBarWidget;

  UPROPERTY(VisibleAnywhere)
  UPawnSensingComponent* PawnSensing;

  UPROPERTY(EditAnywhere)
  TSubclassOf<class AWeapon> Weaponclass;

  /**
  * Combat Property
  */


  UPROPERTY()
  AActor* CombatTarget;

  UPROPERTY(EditAnywhere)
  float CombatRadius = 500.f;

  UPROPERTY(EditAnywhere)
  float AttackRadius = 150.f;

  /*
  *  Navigation
  */
  UPROPERTY()
  class AAIController* EnemyController;

  //current patrol target
  UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
  AActor* PatrolTarget;

  UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
  TArray<AActor*> PatrolTargets;

  UPROPERTY(EditAnywhere)
  float PatrolTargetRadius = 100.f;

  FTimerHandle PatrolTimer;
  void PatrolTimerFinished();

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  float WaitMin = 5.f;

  UPROPERTY(EditAnywhere, Category = "AI Navigation")
  float WaitMax = 10.f;

  /** AI Behavior **/
  void HideHealthBar();
  void ShowHealthBar();
  void LoseInterest();
  void StartPatrolling();
  void ChaseTarget();
  bool IsOutsideCombatRadius();
  bool IsOutsideAttackRadius();
  bool IsInsideAttackRadius();
  bool IsChasing();
  bool IsAttacking();
  bool IsDead();
  bool IsEngaged();
  void ClearPatrolTimer();

  /** Combat **/
  void StartAttackTimer();
  void ClearAttackTimer();

  FTimerHandle AttackTimer;

  UPROPERTY(EditAnywhere, Category = Combat)
  float AttackMin = 0.5f;
  UPROPERTY(EditAnywhere, Category = Combat)
  float AttackMax = 1.f;
  UPROPERTY(EditAnywhere, Category = Combat)
  float PatrollingSpeed = 125.f;
  UPROPERTY(EditAnywhere, Category = Combat)
  float ChasingSpeed = 300.f;
};
