// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "OpenWorldCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;

UCLASS()
class OPENWORLDA_API AOpenWorldCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOpenWorldCharacter();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  /**
  * CallBack Input Funtion
  */
  void MoveForward(float value);
  void Turn(float value);
  void LookUp(float value);
  void MoveLeftAndRight(float value);
  void Equipment();
  virtual void Attack() override;

  /**
  * Play Montage Function
  */
  virtual void PlayAttackMontage() override;

  virtual bool CanAttack() override;

  virtual void AttackEnd() override;

  void PlayEquipMontage(FName SectionName);
  bool CanDisarm();
  bool CanArm();

private:

  ECharacterState CharacterState = ECharacterState::ECS_Uequipped;
  
  UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
  EActionState ActionState = EActionState::EAS_Unoccupied;

  UPROPERTY(VisibleAnywhere)
  USpringArmComponent* CameraBoom;

  UPROPERTY(VisibleAnywhere)
  UCameraComponent* ViewCamera;

  UPROPERTY(EditDefaultsOnly, Category = GroomHair)
  UGroomComponent* Hair;

  UPROPERTY(EditDefaultsOnly, Category = GroomHair)
  UGroomComponent* Eyebrows;

  UPROPERTY(VisibleInstanceOnly)
  AItem* OverlappingItem;


  /**
  Montages Animation
  */

  UPROPERTY(EditDefaultsOnly, Category = Montages)
  UAnimMontage* EquipMontage;

public:
  FORCEINLINE void SetOverLappingItem(AItem* item) { OverlappingItem = item; }
  FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
 