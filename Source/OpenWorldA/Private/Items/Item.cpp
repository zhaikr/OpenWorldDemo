// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "DrawDebugHelpers.h"
#include "OpenWorldA/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/OpenWorldCharacter.h"
#include "NiagaraComponent.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
  ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  RootComponent = ItemMesh;

  Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
  Sphere->SetupAttachment(GetRootComponent());

  EmberEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
  EmberEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	//UWorld* World = GetWorld();
	//SetActorLocation(FVector(0.f, 0.f, 150.f));
	//SetActorRotation(FRotator(0, 45.f, 0));

	//FVector Location = GetActorLocation();
	//FVector Forward_Location = 100.f * GetActorForwardVector()+Location;

	//DRAW_DEBUGSPHERE(Location);
	//DRAW_DEBUGLINE(Location, Forward_Location);
	//DRAW_DEBUGPOINT(Location);
	//DRAW_DEBUGVECTOR(Location, Forward_Location);

  Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverLapStart);
  Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverLapEnd);

}

float AItem::TransformedSin(float value)
{
  float DeltaZ = Amplitude * FMath::Sin(value * TimeConstant); // period = 2Π/k
  return DeltaZ;
}

float AItem::TransformedCos(float value)
{
  float DeltaZ = Amplitude * FMath::Cos(value * TimeConstant); // period = 2Π/k
  return DeltaZ;
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("this is delta time: %d"), DeltaTime);
	RunningTime += DeltaTime;
	//float PerFPS_MovementRate = 50.f * RunningTime;
	//float PerFPS_RotationRate = 45.f * RunningTime;

  //UE_LOG(LogTemp, Warning, TEXT("this is sin value: %d"), TransformedSin(RunningTime));
	//AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin(RunningTime)));


	//AddActorWorldRotation(FRotator(0.f, PerFPS_RotationRate, 0.f));
  if (ItemState == EItemStates::EIS_Hovering)
  {
    const float z = TransformedSin(RunningTime);
    AddActorWorldOffset(FVector(0.f, 0.f, z));
  }


}

void AItem::OnSphereOverLapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  const FString othername = OtherActor->GetName();
  //if (GEngine)
  //{
  //  GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, othername);
  //}
  AOpenWorldCharacter* OWcharacter = Cast<AOpenWorldCharacter>(OtherActor);
  if (OWcharacter)
  {
    OWcharacter->SetOverLappingItem(this);
  }
}

void AItem::OnSphereOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  FString othername = FString::Printf(TEXT("OtherBodyIndex: %d"), OtherBodyIndex);

  //if (GEngine)
  //{
  //  GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, othername);
  //}
  AOpenWorldCharacter* OWcharacter = Cast<AOpenWorldCharacter>(OtherActor);
  if (OWcharacter)
  {
    OWcharacter->SetOverLappingItem(nullptr);
  }
}

