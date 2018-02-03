// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceInvadersGameMode.generated.h"


// Forward declarations
class USpaceInvadersGameInstance;
class AEnemy;
class AEnemyBoss;
class ASpawnPoint;
class USoundBase;
class UAudioComponent;

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

	// Used by the UI to know when to create the boss health bar
	UFUNCTION(BlueprintCallable)
	bool GetBossHasSpawned() { return bBossHasSpawned; }

	// Used by the Boss to tell when he has spawned
	UFUNCTION(BlueprintCallable)
	void SetBossHasSpawned() { bBossHasSpawned = true; }

	// Boss sets this when it dies
	UFUNCTION(BlueprintCallable)
	void SetBossIsDead() { bBossWaveDone = true; }

	// Used by the UI to be able to get the health of the boss
	UFUNCTION(BlueprintCallable)
	AEnemyBoss* getEnemyBoss() { return EnemyBoss; }
	UFUNCTION(BlueprintCallable)
	void SetEnemyBoss(AEnemyBoss* Boss) { EnemyBoss = Boss; }

	UPROPERTY(EditAnywhere)
	USoundBase* Music;

	UAudioComponent* CurrentMusic;

private:
	//** Functions **//

	// Sets the basic variables used every game
	ASpaceInvadersGameMode(const FObjectInitializer& ObjectInitializer);

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

	// Iterates through spawnpoints and spawns a boss instead of regular enemies
	void SpawnNewBossWave();

	// Iterates through and counts current enemies on the field
	void SetEnemiesLeftOnField();

	// Returns if the player is dead
	bool bIsPlayerDead();

	// Decrements one enemy from the total number left to spawn
	void SetEnemiesLeftToSpawn();

	// Checks for when the game is over
	void WinCheck();

	//** Variables **//

	AEnemyBoss* EnemyBoss;

	USpaceInvadersGameInstance* GameInstance;

	FTimerHandle SpawnTimerHandle;

	TArray<ASpawnPoint*> SpawnPoints;

	int TotalWaves;

	int TotalWavesLeft;

	int EnemiesLeftToSpawn;
	
	int EnemyShipsKilled;

	int EnemiesLeftOnField;

	bool bIsCurrentlyPlayingMusic;
	
	bool bIsBossWave;

	bool bBossHasSpawned;

	bool bBossWaveDone;

	bool bIsGameOver;

	bool bPlayerWon;

	bool bCanSpawn;

	bool bPlayerHitByEnemy;

	bool bGameIsPaused;

	float SpawnRate;

	float Timer;

};



