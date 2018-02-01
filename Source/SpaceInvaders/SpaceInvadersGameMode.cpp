// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersPawn.h"
#include "Public/EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SpaceInvadersGameInstance.h"
#include "Enemy.h"
#include "SpawnPoint.h"

// Init default variables on construction (variables that cannot be changed through in-game settings)
ASpaceInvadersGameMode::ASpaceInvadersGameMode()
{
	DefaultPawnClass = ASpaceInvadersPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	bCanSpawn = true;
	bPlayerHitByEnemy = false;
	bPlayerIsDead = false;
	bIsGameOver = false;
	bGameIsPaused = false;
	bPlayerWon = false;
	bEnemyHitTrigger = false;
	EnemiesLeftOnField = 0;
	EnemyShipsKilled = 0;
}

// Init the game variables that has to be set at runtime
void ASpaceInvadersGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Gets the current game instance
	GameInstance = Cast<USpaceInvadersGameInstance>(GetWorld()->GetGameInstance());

	// Finds all spawnpoints
	int numberOfSpawns = FindAllSpawnPoints();

	// Sets current number of waves
	TotalWaves = GameInstance->GetTotalWaves();

	// Sets number of enemies left to spawn (-1 to remove the boss spawner)
	TotalWavesLeft = TotalWaves;
	EnemiesLeftToSpawn = numberOfSpawns * TotalWavesLeft - 1;

	// Sets the spawnrate
	SpawnRate = GameInstance->GetSpawnRate();

	if (Music != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Music);
	}
}

void ASpaceInvadersGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// For the countdown timer
	if (!bCanSpawn)
	{
		Timer = Timer - DeltaSeconds * 1;
	}

	// Updates the current Enemies on  field to the current number
	SetEnemiesLeftOnField();

	WinCheck();

	// Spawns a new wave if conditions allow
	if (bCanSpawn && TotalWavesLeft != 0)
	{
		bCanSpawn = false;
		SpawnNewWave();
		// Resets the timer, removes a wave and starts the timer handle for bCanSpawn
		if (TotalWaves >= 1 && !bIsBossWave)
		{
			Timer = SpawnRate;
			TotalWavesLeft--;
			if (TotalWavesLeft == 0)
			{
				bIsBossWave = true;
				SpawnRate = 11;
				Timer = SpawnRate;
			}
		}

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpaceInvadersGameMode::CanNowSpawnNewShip, SpawnRate);
	}
	// if next wave is the boss wave, only spawn the boss
	else if (bCanSpawn && bIsBossWave)
	{
		SpawnNewBossWave();
	}
}

// Finds all Spawnpoints, adds them to the array and return the number
int ASpaceInvadersGameMode::FindAllSpawnPoints()
{
	int temp = 0;
	for (TActorIterator<ASpawnPoint> It(GetWorld(), ASpawnPoint::StaticClass()); It; ++It)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(*It);
		SpawnPoints.Add(SpawnPoint);
		temp++;
	}
	return temp;
}

// Loops through all spawnpoints and spawns an enemy (excludes the boss)
void ASpaceInvadersGameMode::SpawnNewWave()
{
	for (ASpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (!SpawnPoint->GetIfBossSpawnpoint())
		{
			SpawnPoint->SpawnEnemyShip();
			SetEnemiesLeftToSpawn();
		}
	}
}
// Goes through the spawnpoints to spawn the boss (only one spawnpoint has the bool set to true)
void ASpaceInvadersGameMode::SpawnNewBossWave()
{
	for (ASpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint->GetIfBossSpawnpoint())
		{
			SpawnPoint->SpawnEnemyShip();
		}
	}
}

void ASpaceInvadersGameMode::WinCheck()
{
	// If the player is dead or an enemy reaches the end, the player loses
	if (bIsPlayerDead() || bEnemyHitTrigger)
	{
		bIsGameOver = true;
		GetWorld()->GetFirstPlayerController()->Pause();
	}

	// If there are no enemies left on the field, there are no more waves and no enemies reached the end
	// and the boss is dead, the player wins

	if (EnemiesLeftOnField == 0 && bBossWaveDone && !bEnemyHitTrigger)
	{
		bIsGameOver = true;
		bPlayerWon = true;
		GetWorld()->GetFirstPlayerController()->Pause();
	}
}

// Iterates through all enemies on the field
void ASpaceInvadersGameMode::SetEnemiesLeftOnField()
{
	int temp = 0;

	for (TActorIterator<AEnemy> It(GetWorld(), AEnemy::StaticClass()); It; ++It)
	{
		temp++;
	}

	EnemiesLeftOnField = temp;
}

/* Only getters and setters used throughout the game below */

int ASpaceInvadersGameMode::GetWavesLeft()
{
	return TotalWavesLeft;
}

int ASpaceInvadersGameMode::GetEnemiesLeftToSpawn()
{
	return EnemiesLeftToSpawn;
}

int ASpaceInvadersGameMode::GetShipsKilled()
{
	return EnemyShipsKilled;
}

int ASpaceInvadersGameMode::GetEnemiesLeftOnField()
{
	return EnemiesLeftOnField;
}

int ASpaceInvadersGameMode::GetTimerToSpawn()
{
	return FMath::RoundToZero(Timer);
}

bool ASpaceInvadersGameMode::CheckIfGameIsOver()
{
	return bIsGameOver;
}

bool ASpaceInvadersGameMode::bIsPlayerDead()
{
	return bPlayerIsDead;
}

bool ASpaceInvadersGameMode::GetPlayerWon()
{
	return bPlayerWon;
}

bool ASpaceInvadersGameMode::CheckIfGameIsPaused()
{
	return bGameIsPaused;
}

void ASpaceInvadersGameMode::SetShipsKilled()
{
	EnemyShipsKilled++;
}

void ASpaceInvadersGameMode::SetGameIsPaused()
{
	GetWorld()->GetFirstPlayerController()->SetPause(true);
	bGameIsPaused = true;
}

void ASpaceInvadersGameMode::SetGameIsNotPaused()
{
	GetWorld()->GetFirstPlayerController()->SetPause(false);
	bGameIsPaused = false;
}

void ASpaceInvadersGameMode::SetEnemiesLeftToSpawn()
{
	EnemiesLeftToSpawn--;
}

void ASpaceInvadersGameMode::CanNowSpawnNewShip()
{
	bCanSpawn = true;
}
