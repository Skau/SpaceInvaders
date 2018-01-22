// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SpawnPoint.generated.h"

// Forward declarations
class AEnemy;

/**
 * Spawns an enemy at the location of the spawn point when the gamemode tells it to
 */
UCLASS()
class SPACEINVADERS_API ASpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	ASpawnPoint();

	void SpawnEnemyShip();

	TSubclassOf<AEnemy> Enemy_BP;
};
