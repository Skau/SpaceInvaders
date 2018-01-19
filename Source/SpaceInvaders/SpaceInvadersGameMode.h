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
	UFUNCTION(BlueprintCallable)
	bool CheckIfGameIsPaused();

	// Used to increment the kill counter by the projectile
	void SetShipsKilled();

	// Variables used for the wincheck, set from Player Pawn and Enemy Actor respectively
	bool bPlayerIsDead;
	bool bEnemyHitTrigger;

	// Used by the UI to resume the game
	UFUNCTION(BlueprintCallable)
	void SetGameIsNotPaused();

	// Used by the player controller (action binding) to pause the game
	void SetGameIsPaused();

private:
	//** Functions **//

	// Sets the basic variables used every game
	ASpaceInvadersGameMode();

	// Sets a few variables that cannot be set in the constructor
	virtual void BeginPlay() override;

	// Updates variables, spawns enemies and checks if the game is over
	virtual void Tick(float DeltaSeconds) override;

	// Used to spawn enemy ships
	int FindAllSpawnPoints();

	// Used by the timerhandle to set spawnrate
	void CanNowSpawnNewShip();
	
	// Iterates through spawnpoints and calls the spawn ship function
	void SpawnNewWave();

	// Iterates through and counts current enemies on the field
	void SetEnemiesLeftOnField();

	// Returns if the player is dead
	bool bIsPlayerDead();

	// Decrements one enemy from the total number left to spawn
	void SetEnemiesLeftToSpawn();

	// Checks for when the game is over
	void WinCheck();

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

	bool bGameIsPaused;

	float SpawnRate;

	float Timer;

};



