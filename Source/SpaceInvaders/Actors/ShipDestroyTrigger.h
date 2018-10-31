// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipDestroyTrigger.generated.h"

class UBoxComponent;

UCLASS()
class SPACEINVADERS_API AShipDestroyTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AShipDestroyTrigger();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UBoxComponent* TriggerVolume = nullptr;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
