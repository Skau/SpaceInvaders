// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SpawnPoint.generated.h"

// Forward declarations
class AEnemy;
class AEnemyBoss;
/**
 * Spawns an enemy at the location of the spawn point when the gamemode tells it to
 */
UCLASS()
class SPACEINVADERS_API ASpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ASpawnPoint();

	// Spawns as hip
	void SpawnEnemyShip();

	// Used to see if this is the boss spawnpoint
	bool GetIfBossSpawnpoint() { return bIsBossSP; }

	TSubclassOf<AEnemy> Enemy_BP;
	TSubclassOf<AEnemyBoss> EnemyBoss_BP;

	// If this is checked in details, nothing will spawn unless it's the boss wave (only one spawnpoint has this checked)
	UPROPERTY(EditAnywhere)
	bool bIsBossSP;

	// Used by the gamemode to create the boss health bar
	bool bHaveSpawnedBoss;
};
