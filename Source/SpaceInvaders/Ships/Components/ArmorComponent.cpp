// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorComponent.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UArmorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UArmorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

