// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "OpenWorldA/DebugMacros.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
  GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
  GetMesh()->SetGenerateOverlapEvents(true);

  GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

  HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
  HealthBarWidget->SetupAttachment(GetRootComponent());

  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;

  PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSeenComponent"));
  PawnSensing->SightRadius = 4000.f;
  PawnSensing->SetPeripheralVisionAngle(45.f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
  if (HealthBarWidget)
  {
    HealthBarWidget->SetVisibility(false);
  }
  EnemyController = Cast<AAIController>(GetController());

  MoveToTarget(PatrolTarget);
  

  if (PawnSensing)
  {
    PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
  }

  UWorld* World = GetWorld();
  if (World && Weaponclass)
  {
    AWeapon* DeafaultWeaapon = World->SpawnActor<AWeapon>(Weaponclass);
    DeafaultWeaapon->Equipment(GetMesh(), FName("righthandsocket"), this, this);
    EquipedWeapon = DeafaultWeaapon;
  }

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (IsDead()) return;

  if (EnemyState > EEnemyState::EES_Patrolling)
  {
    CheckCombatTarget();
  }
  else
  {
    CheckPatroltarget();
  }

}

void AEnemy::CheckCombatTarget()
{
  //check enemy fighting HealthBarWidget is or not show, if over the CombatRadius
  if (IsOutsideCombatRadius())
  {
    //outside combat radius, lose interest
    ClearAttackTimer();
    LoseInterest();
    if (!IsEngaged())
    {
      StartPatrolling();
    }

    //UE_LOG(LogTemp, Warning, TEXT("Lose Interest"))
  }
  else if (IsOutsideAttackRadius() && !IsChasing())
  {
    //outside Attack radius, chasing character
    ClearAttackTimer();
    if (!IsEngaged())
    {
      ChaseTarget();
    }

    //UE_LOG(LogTemp, Warning, TEXT("Chase player"))
  }
  else if (CanAttack())
  {
    //Inside Attack radius, Attacking character
    //TODO: Attack montage
    //UE_LOG(LogTemp, Warning, TEXT("Attack player"))
    StartAttackTimer();
  }
}

void AEnemy::CheckPatroltarget()
{
  //Checking  Partolpoints 
  if (InTargetRange(PatrolTarget, PatrolTargetRadius))
  {
    PatrolTarget = ChoosePatrolTarget();
    const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
    GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
  }
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Attack()
{
  Super::Attack();
  PlayAttackMontage();
}

void AEnemy::PlayAttackMontage()
{
  Super::PlayAttackMontage();
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && AttackMontage)
  {
    AnimInstance->Montage_Play(AttackMontage);
    int32 Selection = FMath::RandRange(0, 2);
    FName SeletionName;
    switch (Selection)
    {
    case 0:
      SeletionName = FName("Attack1");
      break;
    case 1:
      SeletionName = FName("Attack2");
      break;
    case 2:
      SeletionName = FName("Attack3");
      break;
    default:
      break;
    }
    AnimInstance->Montage_JumpToSection(SeletionName, AttackMontage);
  }
}

bool AEnemy::CanAttack()
{
  bool bCanAttack =
    IsInsideAttackRadius() &&
    !IsAttacking() &&
    !IsDead();
  return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
  Super::HandleDamage(DamageAmount);

  if (Attributes && HealthBarWidget)
  {
    HealthBarWidget->SetHealthPercentage(Attributes->GetHealthPercent());
  }
}

void AEnemy::Die()
{

  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && DeathMontage)
  {
    AnimInstance->Montage_Play(DeathMontage);
    int32 Selection = FMath::RandRange(0, 5);
    FName SeletionName;
    switch (Selection)
    {
    case 0:
      SeletionName = FName("Death1");
      DeathPose = EDeathPose::EDP_Death1;
      break;
    case 1:
      SeletionName = FName("Death2");
      DeathPose = EDeathPose::EDP_Death2;
      break;
    case 2:
      SeletionName = FName("Death3");
      DeathPose = EDeathPose::EDP_Death3;
      break;
    case 3:
      SeletionName = FName("Death4");
      DeathPose = EDeathPose::EDP_Death4;
      break;
    case 4:
      SeletionName = FName("Death5");
      DeathPose = EDeathPose::EDP_Death5;
      break;
    case 5:
      SeletionName = FName("Death6");
      DeathPose = EDeathPose::EDP_Death6;
      break;
    default:
      break;
    }
    AnimInstance->Montage_JumpToSection(SeletionName, DeathMontage);
  }

  if (HealthBarWidget)
  {
    HealthBarWidget->SetVisibility(false);
  }
  GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  SetLifeSpan(5.f);

}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
  if (Target == nullptr) return false;
  const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
  //UE_LOG(LogTemp, Warning, TEXT("The float value is: %f"), DistanceToTarget);
  //DRAW_DEBUGSPHERE_SINGLEFRAME(Target->GetActorLocation())
  //DRAW_DEBUGSPHERE_SINGLEFRAME(GetActorLocation())
  return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
  if (EnemyController == nullptr || Target == nullptr) return;
  FAIMoveRequest MoveRequset;
  MoveRequset.SetGoalActor(Target);
  MoveRequset.SetAcceptanceRadius(60.f);
  FNavPathSharedPtr NavPath;
  EnemyController->MoveTo(MoveRequset, &NavPath);
  //TArray<FNavPathPoint>& PathPoinst = NavPath->GetPathPoints();
  //for (auto& point : PathPoinst)
  //{
  //  const FVector& Location = point.Location;
  //  DRAW_DEBUGSPHERE(Location);
  //}
}

AActor* AEnemy::ChoosePatrolTarget()
{
  TArray<AActor*> ValidTargets;
  for (auto& target : PatrolTargets)
  {
    if (target != PatrolTarget)
    {
      ValidTargets.AddUnique(target);
    }
  }
  const int32 NumPatrolTargets = ValidTargets.Num();
  if (NumPatrolTargets > 0)
  {
    const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
    AActor* Target = ValidTargets[TargetSelection];
    return Target;
  }

  return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
  const bool bShouldChaseTarget =
    EnemyState != EEnemyState::EES_Dead &&
    EnemyState != EEnemyState::EES_Chasing &&
    EnemyState < EEnemyState::EES_Attacking &&
    SeenPawn->ActorHasTag("OpenWorldCharacter");

  if (bShouldChaseTarget)
  {
    CombatTarget = SeenPawn;
    ClearPatrolTimer();
    ChaseTarget();
    //UE_LOG(LogTemp, Warning, TEXT("Pawn Seen, chase player!!"));
  }

}

void AEnemy::PatrolTimerFinished()
{
  MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
  if (HealthBarWidget)
  {
    HealthBarWidget->SetVisibility(false);
  }
}

void AEnemy::ShowHealthBar()
{
  if (HealthBarWidget)
  {
    HealthBarWidget->SetVisibility(true);
  }
}

void AEnemy::LoseInterest()
{
  CombatTarget = nullptr;
  HideHealthBar();
}

void AEnemy::StartPatrolling()
{
  EnemyState = EEnemyState::EES_Patrolling;
  GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
  MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
  EnemyState = EEnemyState::EES_Chasing;
  GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
  MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
  return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
  return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
  return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
  return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
  return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
  return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
  return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
  GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartAttackTimer()
{
  EnemyState = EEnemyState::EES_Attacking;
  const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
  GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
    GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
  //DRAW_DEBUGSPHERECOLOR(ImpactPoint, FColor::Orange);
  ShowHealthBar();

  if (IsAlive())
  {
    DirectionHirReact(ImpactPoint);
  }
  else
  {
    Die();
  }


  PlayHitSound(ImpactPoint);

  SpawnHitParticles(ImpactPoint);

}

//inherit from AActor
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
  HandleDamage(DamageAmount);
  CombatTarget = EventInstigator->GetPawn();
  ChaseTarget();
  return DamageAmount;
}

void AEnemy::Destroyed()
{
  if (EquipedWeapon)
  {
    EquipedWeapon->Destroy();
  }
}

