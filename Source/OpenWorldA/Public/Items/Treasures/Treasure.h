// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDA_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
  virtual void OnSphereOverLapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
  UPROPERTY(EditAnywhere, Category = Sounds)
  USoundBase* PickUpSound;

  UPROPERTY(EditAnywhere, Category = "Treature Properties")
  int32 Gold;

};
