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

	bool GetHaveSpawnedBoss() { return bHaveSpawnedBoss; }
	void SetHaveSpawnedBoss(bool Value) { bHaveSpawnedBoss = Value; }

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> Enemy_BP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyBoss> EnemyBoss_BP;

	UPROPERTY(EditAnywhere)
	bool bIsBossSP;

private:
	bool bHaveSpawnedBoss;
};
