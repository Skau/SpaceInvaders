// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvadersGameMode.h"
#include "EndlessGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AEndlessGameMode : public ASpaceInvadersGameMode
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	int GetCurrentWave() { return CurrentWave; }

private:
	AEndlessGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;


	int CurrentWave;
	int CurrentWaveCounter;
};
