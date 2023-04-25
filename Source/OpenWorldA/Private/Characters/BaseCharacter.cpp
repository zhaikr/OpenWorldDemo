// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

bool ABaseCharacter::CanAttack()
{
  return false;
}

bool ABaseCharacter::IsAlive()
{
  return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
  if (Attributes)
  {
    Attributes->ReceiveDamage(DamageAmount);
  }
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
  if (HitSound)
  {
    UGameplayStatics::PlaySoundAtLocation(
      this,
      HitSound,
      ImpactPoint
    );
  }
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
  if (HitParticles)
  {
    UGameplayStatics::SpawnEmitterAtLocation(
      GetWorld(),
      HitParticles,
      ImpactPoint
    );
  }
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{ 
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && HitReactMontage)
  {
    AnimInstance->Montage_Play(HitReactMontage);
    AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
  }
}

void ABaseCharacter::DirectionHirReact(const FVector& ImpactPoint)
{

  const FVector Forward = GetActorForwardVector();
  const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);

  const FVector ToHit = (ImpactLowered - GetActorLocation()).GetUnsafeNormal();
  // Forward * ToHit = |Forward||ToHit| * COS(theta)
  // |Forward| = 1, |ToHit| = 1  Forward * ToHit = cos(theta)
  const double CosTheta = FVector::DotProduct(Forward, ToHit);

  //take the inverse cosine (arc-cos) of cos(theta) to get theta
  double theta = FMath::Acos(CosTheta);

  //convert from radiance to degrees

  theta = FMath::RadiansToDegrees(theta);

  //if CrossProdut points down theta should be negative
  const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
  if (CrossProduct.Z < 0.f)
  {
    theta *= -1.f;
  }

  FName Section("FromBack");
  if (theta >= -45.f && theta <= 45.f)
  {
    Section = FName("FromFront");
  }
  else if (theta > 45.f && theta <= 135.f)
  {
    Section = FName("FromRight");
  }
  else if (theta >= -135.f && theta < -45.f)
  {
    Section = FName("FromLeft");
  }

  PlayHitReactMontage(Section);

  /*
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.f, FColor::Blue, 5.f);

  if (GEngine)
  {
    GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("theta : %f"), theta));
  }

  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
  UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
  */
}

void ABaseCharacter::AttackEnd()
{
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
  if (EquipedWeapon && EquipedWeapon->GetWeaponBox())
  {
    EquipedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
    EquipedWeapon->IgnoreActors.Empty();
  }
}
