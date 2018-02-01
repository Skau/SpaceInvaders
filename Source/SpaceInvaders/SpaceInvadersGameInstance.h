// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceInvadersGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API USpaceInvadersGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int GetTotalWaves();

	UFUNCTION(BlueprintCallable)
	void SetTotalWaves(int NewTotalWaves);

	UFUNCTION(BlueprintCallable)
	int GetSpawnRate();

	UFUNCTION(BlueprintCallable)
	void SetSpawnRate(int NewSpawnRate);

private:
	int TotalWaves = 10;

	int SpawnRate = 3;

	
	
};
