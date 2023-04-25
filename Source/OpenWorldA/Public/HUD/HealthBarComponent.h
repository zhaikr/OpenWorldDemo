// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDA_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
  void SetHealthPercentage(float Percentage);

private:
  UPROPERTY()
  class UHealthBar* HealthBarWidget;
};
