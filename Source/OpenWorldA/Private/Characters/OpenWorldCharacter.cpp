// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OpenWorldCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

// Sets default values
AOpenWorldCharacter::AOpenWorldCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;
  GetCharacterMovement()->bOrientRotationToMovement = true;
  GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

  CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
  CameraBoom->SetupAttachment(GetRootComponent());
  CameraBoom->TargetArmLength = 300.f;

  ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
  ViewCamera->SetupAttachment(CameraBoom);

  Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
  Hair->SetupAttachment(GetMesh());
  Hair->AttachmentName = FString("head");

  Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
  Eyebrows->SetupAttachment(GetMesh());
  Eyebrows->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void AOpenWorldCharacter::BeginPlay()
{
	Super::BeginPlay();
  Tags.Add(FName("OpenWorldCharacter"));
}


// Called every frame
void AOpenWorldCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOpenWorldCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
  PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AOpenWorldCharacter::MoveForward);
  PlayerInputComponent->BindAxis(FName("MoveLeftAndRight"), this, &AOpenWorldCharacter::MoveLeftAndRight);
  PlayerInputComponent->BindAxis(FName("Turn"), this, &AOpenWorldCharacter::Turn);
  PlayerInputComponent->BindAxis(FName("LookUp"), this, &AOpenWorldCharacter::LookUp);
  PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AOpenWorldCharacter::Equipment);
  PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AOpenWorldCharacter::Attack);

}

void AOpenWorldCharacter::MoveForward(float value)
{
  if (ActionState == EActionState::EAS_Attacking) { return; }
  if (Controller && value != 0.f)
  {
    //FVector Forward = GetActorForwardVector();
    //AddMovementInput(Forward, value);
    const FRotator ControlRotator = GetControlRotation();
    const FRotator YawRotation(0.f, ControlRotator.Yaw, 0.f);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, value);
  }
}

void AOpenWorldCharacter::MoveLeftAndRight(float value)
{
  if (ActionState == EActionState::EAS_Attacking) { return; }
  if (Controller && value != 0.f)
  {
    //FVector Right = GetActorRightVector();
    //AddMovementInput(Right, value);
    const FRotator ControlRotator = GetControlRotation();
    const FRotator YawRotation(0.f, ControlRotator.Yaw, 0.f);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, value);
  }
}

void AOpenWorldCharacter::Equipment()
{
  AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
  if (OverlappingWeapon)
  {
    OverlappingWeapon->Equipment(GetMesh(), FName("righthandsocket"), this, this);
    CharacterState = ECharacterState::ECS_EquippedSingleWeapon;
    EquipedWeapon = OverlappingWeapon;
  }
  else
  {
    if (CanDisarm())
    {
      PlayEquipMontage(FName("Unequip"));
      CharacterState = ECharacterState::ECS_Uequipped;
    }
    else if (CanArm())
    {
      PlayEquipMontage(FName("Equip"));
      CharacterState = ECharacterState::ECS_EquippedSingleWeapon;
    }
  }


}


void AOpenWorldCharacter::Turn(float value)
{
  AddControllerYawInput(value);
}

void AOpenWorldCharacter::LookUp(float value)
{
  AddControllerPitchInput(value);
}

void AOpenWorldCharacter::Attack()
{
  Super::Attack();
  if (CanAttack())
  {
    PlayAttackMontage();
    ActionState = EActionState::EAS_Attacking;
  }

}

void AOpenWorldCharacter::PlayAttackMontage()
{
  Super::PlayAttackMontage();
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && AttackMontage)
  {
    AnimInstance->Montage_Play(AttackMontage);
    int32 Selection = FMath::RandRange(0, 1);
    FName SeletionName;
    switch (Selection)
    {
      case 0:
        SeletionName = FName("Attack1");
        break;
      case 1:
        SeletionName = FName("Attack2");
        break;
      default:
        break;
    }
    AnimInstance->Montage_JumpToSection(SeletionName, AttackMontage);
  }
}

void AOpenWorldCharacter::PlayEquipMontage(FName SectionName)
{
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && EquipMontage)
  {
    AnimInstance->Montage_Play(EquipMontage);
    AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
  }
}

void AOpenWorldCharacter::AttackEnd()
{
  ActionState = EActionState::EAS_Unoccupied;
}

bool AOpenWorldCharacter::CanAttack()
{
  return  ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Uequipped;
}


bool AOpenWorldCharacter::CanDisarm()
{
  return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Uequipped ;
}

bool AOpenWorldCharacter::CanArm()
{
  return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Uequipped && EquipedWeapon;
}
