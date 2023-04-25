// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasures/Treasure.h"
#include "Characters/OpenWorldCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverLapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  AOpenWorldCharacter* OWcharacter = Cast<AOpenWorldCharacter>(OtherActor);
  if (OWcharacter)
  {
    if (PickUpSound)
    {
      UGameplayStatics::PlaySoundAtLocation(
        this,
        PickUpSound,
        GetActorLocation()
      );
    }

    Destroy();
  }
}
