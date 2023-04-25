// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/OpenWorldCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OpenWorldA/Public/Interfaces/HitInterface.h"
#include "NiagaraComponent.h"


AWeapon::AWeapon()
{
  WeaponBox = CreateDefaultSubobject<UBoxComponent>("Weapon Box");
  WeaponBox->SetupAttachment(GetRootComponent());
  WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

  BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
  BoxTraceStart->SetupAttachment(GetRootComponent());

  BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
  BoxTraceEnd->SetupAttachment(GetRootComponent());

}

void AWeapon::BeginPlay()
{
  Super::BeginPlay();

  WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereOverLapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  Super::OnSphereOverLapStart(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,  bFromSweep, SweepResult);
  AOpenWorldCharacter* OWcharacter = Cast<AOpenWorldCharacter>(OtherActor);
  //if (OWcharacter)
  //{
  //  FAttachmentTransformRules TransformRule(EAttachmentRule::SnapToTarget, true);
  //  ItemMesh->AttachToComponent(OWcharacter->GetMesh(), TransformRule, FName("righthandsocket"));
  //}
}

void AWeapon::OnSphereOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  Super::OnSphereOverLapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}



void AWeapon::Equipment(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
  SetOwner(NewOwner);
  SetInstigator(NewInstigator);
  FAttachmentTransformRules TransformRule(EAttachmentRule::SnapToTarget, true);
  ItemMesh->AttachToComponent(InParent, TransformRule, SocketName);
  ItemState = EItemStates::EIS_Equipped;
  if (EquipSound)
  {
    UGameplayStatics::PlaySoundAtLocation(
      this,
      EquipSound,
      GetActorLocation()
    );
  }
  if (Sphere)
  {
    Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  }

  if (EmberEffect)
  {
    EmberEffect->Deactivate(); 
  }
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  const FVector Start = BoxTraceStart->GetComponentLocation();
  const FVector End = BoxTraceEnd->GetComponentLocation();

  TArray<AActor*> ActorToIgnore;
  ActorToIgnore.Add(this);

  for (AActor* Actor : IgnoreActors)
  {
    ActorToIgnore.AddUnique(Actor);
  }

  FHitResult BoxHit;
  UKismetSystemLibrary::BoxTraceSingle(
    this,
    Start,
    End,
    FVector(5.f, 5.f, 5.f),
    BoxTraceStart->GetComponentRotation(),
    ETraceTypeQuery::TraceTypeQuery1,
    false,
    ActorToIgnore,
    EDrawDebugTrace::None,
    BoxHit,
    true
  );
  if (BoxHit.GetActor())
  {
    UE_LOG(LogTemp, Warning, TEXT("ATTACKING !!!!!!!"))
    UGameplayStatics::ApplyDamage(
      BoxHit.GetActor(),
      Damage,
      GetInstigator()->GetController(),
      this,
      UDamageType::StaticClass()
    );

    IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
    if (HitInterface)
    {
      HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
    }
    IgnoreActors.AddUnique(BoxHit.GetActor());

    CreateFields(BoxHit.ImpactPoint);
  }
}
