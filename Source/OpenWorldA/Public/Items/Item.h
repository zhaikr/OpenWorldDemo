// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
enum class EItemStates : uint8
{
  EIS_Hovering,
  EIS_Equipped
};

UCLASS()
class OPENWORLDA_API AItem : public AActor
{ 
  GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UFUNCTION()
  virtual void OnSphereOverLapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION()
  virtual void OnSphereOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  UFUNCTION(BlueprintPure) 
  float TransformedSin(float value);

  UFUNCTION(BlueprintPure)
  float TransformedCos(float value);
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameter")
	float Amplitude = 0.5f;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  UStaticMeshComponent* ItemMesh;

  UPROPERTY(VisibleAnywhere)
  USphereComponent* Sphere;

  EItemStates ItemState = EItemStates::EIS_Hovering;

  UPROPERTY(EditAnywhere)
  class UNiagaraComponent* EmberEffect;

private:
	UPROPERTY(VisibleAnywhere)
	float RunningTime;

	UPROPERTY(VisibleDefaultsOnly)
	float TimeConstant = 5.f;



};
