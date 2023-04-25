// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UAnimMontage;
class AWeapon;
class UAttributeComponent;

UCLASS()
class OPENWORLDA_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
  void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:

  /*
  *  Components
  */

  UPROPERTY(VisibleAnywhere)
  UAttributeComponent* Attributes;

  UPROPERTY(EditAnywhere, Category = Sounds)
  USoundBase* HitSound;

  UPROPERTY(EditAnywhere, Category = VisualEffect)
  UParticleSystem* HitParticles;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
  
  virtual void Attack();

  virtual void Die();

  /**
  * Play Montage Function
  */
  virtual void PlayAttackMontage();

  virtual bool CanAttack();

  bool IsAlive();

  virtual void HandleDamage(float DamageAmount);

  void PlayHitSound(const FVector& ImpactPoint);

  void SpawnHitParticles(const FVector& ImpactPoint);

  void PlayHitReactMontage(const FName& SectionName);

  void DirectionHirReact(const FVector& ImpactPoint);

  UFUNCTION(BlueprintCallable)
  virtual void AttackEnd();

  UPROPERTY(VisibleAnywhere, Category = Weapon)
  AWeapon* EquipedWeapon;


  /**
  Montages Animation
  */
  UPROPERTY(EditDefaultsOnly, Category = Montages)
  UAnimMontage* AttackMontage;

  UPROPERTY(EditDefaultsOnly, Category = Montages)
  UAnimMontage* HitReactMontage;

  UPROPERTY(EditDefaultsOnly, Category = Montages)
  UAnimMontage* DeathMontage;

};
