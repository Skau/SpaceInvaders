// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceInvadersGameMode.generated.h"


// Forward declarations
class AEnemy;
class ASpawnPoint;

UCLASS(MinimalAPI)
class ASpaceInvadersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaceInvadersGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	int GetWavesLeft();

	UFUNCTION(BlueprintCallable)
	int GetEnemiesLeftToSpawn();

	UFUNCTION(BlueprintCallable)
	int GetShipsKilled();

	UFUNCTION(BlueprintCallable)
	int GetEnemiesLeftOnField();

	UFUNCTION(BlueprintCallable)
	int GetTimerToSpawn();

	UFUNCTION(BlueprintCallable)
	bool CheckIfGameIsOver();

	UFUNCTION(BlueprintCallable)
	bool GetPlayerWon();

	UFUNCTION(BlueprintCallable)
	int GetTotalWaves();

	void SetShipsKilled();

	void SetEnemiesLeftToSpawn();

	void EndGame();

	bool bPlayerIsDead;

	bool bIsGameOver;

	bool bEnemyHitTrigger;

private:

	void CanNowSpawnNewShip();

	void SetEnemiesLeftOnField();

	bool bIsPlayerDead();

	int TotalWaves;

	int TotalWavesLeft = TotalWaves;

	int EnemiesLeftToSpawn;
	
	int EnemyShipsKilled;

	int EnemiesLeftOnField;

	bool bPlayerWon;

	bool bCanSpawn;

	bool bPlayerHitByEnemy;

	float SpawnRate;

	float Timer = SpawnRate;

	FTimerHandle SpawnTimerHandle;

	TArray<ASpawnPoint*> SpawnPoints;

};



