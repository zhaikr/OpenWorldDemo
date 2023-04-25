// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class OPENWORLDA_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
  AWeapon();
  void Equipment(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
  TArray<AActor*> IgnoreActors;

protected:
  virtual void BeginPlay() override;

  void OnSphereOverLapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

  void OnSphereOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

  UFUNCTION()
  void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION(BlueprintImplementableEvent)
  void CreateFields(const FVector& FieldLocation);

private:
  UPROPERTY(EditAnywhere, Category = "Weapon property")
  USoundBase* EquipSound;

  UPROPERTY(VisibleAnywhere, Category = "Weapon property")
  UBoxComponent* WeaponBox;

  UPROPERTY(VisibleAnywhere)
  USceneComponent* BoxTraceStart;

  UPROPERTY(VisibleAnywhere)
  USceneComponent* BoxTraceEnd;

  UPROPERTY(EditAnywhere, Category = "Weapon property")
  float Damage = 20.f;
public:
  FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
