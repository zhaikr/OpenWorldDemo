// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"


void UHealthBarComponent::SetHealthPercentage(float Percentage)
{
  if (HealthBarWidget == nullptr)
  {
    HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
  }

  if (HealthBarWidget && HealthBarWidget->HealthBar)
  {
    HealthBarWidget->HealthBar->SetPercent(Percentage);
  }
}
