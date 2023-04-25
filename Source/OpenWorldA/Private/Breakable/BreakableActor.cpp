// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "OpenWorldA/Public/Items/Treasures/Treasure.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
  GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
  SetRootComponent(GeometryCollection);
  GeometryCollection->SetGenerateOverlapEvents(true);
  GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
  GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

  Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
  Capsule->SetupAttachment(GetRootComponent());
  Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
  GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::BreakSpawnActor);
	
}

void ABreakableActor::BreakSpawnActor(const FChaosBreakEvent& BreakEvent)
{
  SetLifeSpan(3.f);
  Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

  if (bBroken) { return; }
  bBroken = true;
  UWorld* World = GetWorld();
  if (World && !Treasureclasses.IsEmpty())
  {
    FVector Location = GetActorLocation();
    Location.Z += 75.f;
    int32 Selection = FMath::RandRange(0, Treasureclasses.Num() - 1);
    World->SpawnActor<ATreasure>(Treasureclasses[Selection], Location, GetActorRotation());
  }
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
  if (bBroken) { return; }
  bBroken = true;
  UWorld* World = GetWorld();
  if (World && !Treasureclasses.IsEmpty())
  {
    FVector Location = GetActorLocation();
    Location.Z += 75.f;
    int32 Selection = FMath::RandRange(0, Treasureclasses.Num() - 1);
    World->SpawnActor<ATreasure>(Treasureclasses[Selection], Location, GetActorRotation());
  }
}

