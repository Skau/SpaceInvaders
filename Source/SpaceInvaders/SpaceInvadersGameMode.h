// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceInvadersGameMode.generated.h"


// Forward declarations
class USpaceInvadersGameInstance;
class AEnemy;
class ASpawnPoint;

UCLASS(MinimalAPI)
class ASpaceInvadersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Getters for local variables used by the UI widget during game
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

	// Used to increment the kill counter through the projectile
	void SetShipsKilled();

	// Variables used for the wincheck, set from Player Pawn and Enemy Actor respectively
	bool bPlayerIsDead;
	bool bEnemyHitTrigger;

private:
	//** Functions **//

	ASpaceInvadersGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	int FindAllSpawnPoints();

	void CanNowSpawnNewShip();

	void SpawnNewWave();

	void SetEnemiesLeftOnField();

	bool bIsPlayerDead();

	void SetEnemiesLeftToSpawn();

	void WinCheck();

	void EndGame();

	//** Variables **//

	USpaceInvadersGameInstance* GameInstance;

	FTimerHandle SpawnTimerHandle;

	TArray<ASpawnPoint*> SpawnPoints;

	bool bIsGameOver;

	int TotalWaves;

	int TotalWavesLeft;

	int EnemiesLeftToSpawn;
	
	int EnemyShipsKilled;

	int EnemiesLeftOnField;

	bool bPlayerWon;

	bool bCanSpawn;

	bool bPlayerHitByEnemy;

	float SpawnRate;

	float Timer;

};



