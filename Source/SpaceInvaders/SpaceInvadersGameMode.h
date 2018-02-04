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
class UHighscoreSaver;

USTRUCT(BlueprintType)
struct FHighScoreDataGM
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName = "DefaultPlayerName(GameMode)";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WaveReached = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemiesKilled = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BossesKilled = 0;
};


UCLASS(MinimalAPI)
class ASpaceInvadersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Getters for variables used by the UI widget during game
	UFUNCTION(BlueprintCallable)
	bool GetHighScoreIsOpen() { return bHighScoreIsOpen; }

	UFUNCTION(BlueprintCallable)
	void SetHighScoreIsOpen(bool Value) { bHighScoreIsOpen = Value; }

	UFUNCTION(BlueprintCallable)
	TArray<FHighScoreDataGM> GetHighScores() { return HighScores; }

	UFUNCTION(BlueprintCallable)
	int GetWavesLeft() { return TotalWavesLeft; }

	UFUNCTION(BlueprintCallable)
	int GetEnemiesLeftToSpawn() { return EnemiesLeftToSpawn; }

	UFUNCTION(BlueprintCallable)
	int GetShipsKilled() { return EnemyShipsKilled; }

	UFUNCTION(BlueprintCallable)
	int GetEnemiesLeftOnField() { return EnemiesLeftOnField; }

	UFUNCTION(BlueprintCallable)
	int GetTimerToSpawn() { return FMath::RoundToZero(Timer); }

	UFUNCTION(BlueprintCallable)
	int GetBossKills() { return BossKills; }

	UFUNCTION(BlueprintCallable)
	bool CheckIfGameIsOver() { return bIsGameOver; }

	UFUNCTION(BlueprintCallable)
	bool GetPlayerWon() { return bPlayerWon; }

	UFUNCTION(BlueprintCallable)
	bool CheckIfGameIsPaused() { return bGameIsPaused; }

	UFUNCTION(BlueprintCallable)
	bool GetBossHasSpawned() { return bBossHasSpawned; }

	UFUNCTION(BlueprintCallable)
	bool GetIfBossWave() { return bIsBossWave; }

	UFUNCTION(BlueprintCallable)
	AEnemyBoss* getEnemyBoss() { return EnemyBoss; }

	UFUNCTION(BlueprintCallable)
	UHighscoreSaver* GetLoadedGameObject() { return LoadedGameObject; }

	UFUNCTION(BlueprintCallable)
	void LoadHighScore();

	// The boss sets this when it spawns
	UFUNCTION(BlueprintCallable)
	void SetEnemyBoss(AEnemyBoss* Boss) { EnemyBoss = Boss; }

	// Variables used for the wincheck, set from Player Pawn and Enemy Actor respectively
	bool bPlayerIsDead;
	bool bEnemyHitTrigger;

	// Used to increment the kill counter by the projectile
	void SetShipsKilled() { EnemyShipsKilled++; }

	// Used by the player, boss and projectile to see if allowed to play sounds
	bool GetbIsSoundEffectsAllowed() { return bIsSoundEffectsAllowed; }

	// Used by the UI to resume the game
	UFUNCTION(BlueprintCallable)
	void SetGameIsNotPaused();

	// Used by the player controller (action binding) to pause the game
	void SetGameIsPaused();

	// Used by the Boss to tell when he has spawned
	UFUNCTION(BlueprintCallable)
	void SetBossHasSpawned();
	
	// Boss sets this when it dies
	UFUNCTION(BlueprintCallable)
	void SetBossIsDead();

	// Used to set the background music
	UPROPERTY(BlueprintReadWrite)
	USoundBase* Music;

	UPROPERTY(BlueprintReadWrite)
	TArray<FHighScoreDataGM> HighScores;


protected:
	// Sets the basic variables used every game
	ASpaceInvadersGameMode(const FObjectInitializer& ObjectInitializer);

	// Sets a few variables that cannot be set in the constructor
	virtual void BeginPlay() override;

	// Updates variables, spawns enemies and checks if the game is over
	virtual void Tick(float DeltaSeconds) override;

	// Used to spawn enemy ships
	int FindAllSpawnPoints();

	// Used by the timerhandle to set spawnrate
	void CanNowSpawnNewShip() { bCanSpawn = true; }

	// Iterates through spawnpoints and calls the spawn ship function
	void SpawnNewWave();

	// Iterates through spawnpoints and spawns a boss instead of regular enemies
	void SpawnNewBossWave();

	// Iterates through and counts current enemies on the field
	void SetEnemiesLeftOnField();

	// Returns if the player is dead
	bool bIsPlayerDead() { return bPlayerIsDead; }

	// Decrements one enemy from the total number left to spawn
	void SetEnemiesLeftToSpawn() { EnemiesLeftToSpawn--; }

	//** Variables **//

	UHighscoreSaver* LoadedGameObject;

	UHighscoreSaver* SaveGameObject;

	UAudioComponent* CurrentMusic;

	AEnemyBoss* EnemyBoss;

	USpaceInvadersGameInstance* GameInstance;

	FTimerHandle SpawnTimerHandle;

	TArray<ASpawnPoint*> SpawnPoints;

	int CurrentWave;

	int TotalWaves;

	int TotalWavesLeft;

	int EnemiesLeftToSpawn;
	
	int EnemyShipsKilled;

	int EnemiesLeftOnField;

	int BossKills;

	bool bHighScoreIsOpen;

	bool bIsMusicAllowed;

	bool bIsSoundEffectsAllowed;

	bool bIsClassic;

	bool bIsEndless;

	bool bIsCurrentlyPlayingMusic;
	
	bool bIsBossWave;

	bool bBossHasSpawned;

	bool bBossIsDead;

	bool bIsGameOver;

	bool bPlayerWon;

	bool bCanSpawn;

	bool bPlayerHitByEnemy;

	bool bGameIsPaused;

	float SpawnRate;

	float Timer;

};



