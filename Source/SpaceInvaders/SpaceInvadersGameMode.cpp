// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersPawn.h"
#include "Public/EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "SpawnPoint.h"

ASpaceInvadersGameMode::ASpaceInvadersGameMode()
{
	// Sets default variables
	DefaultPawnClass = ASpaceInvadersPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	bCanSpawn = true;
	bPlayerHitByEnemy = false;
	bPlayerIsDead = false;
	bIsGameOver = false;
	bPlayerWon = false;
	bEnemyHitTrigger = false;
	EnemiesLeftOnField = 0;
	EnemyShipsKilled = 0;
	TotalWaves = 10;
	TotalWavesLeft = TotalWaves;
	SpawnRate = 3.f;
}

void ASpaceInvadersGameMode::BeginPlay()
{
	Super::BeginPlay();

	int temp = 0;
	// Finds all Spawnpoints and adds them to the array
	for (TActorIterator<ASpawnPoint> It(GetWorld(), ASpawnPoint::StaticClass()); It; ++It)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(*It);
		SpawnPoints.Add(SpawnPoint);
		temp++;
	}
	EnemiesLeftToSpawn = temp * TotalWavesLeft;
}

void ASpaceInvadersGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	// For the countdown timer
	if (!bCanSpawn)
	{
		Timer = Timer - DeltaSeconds*1;
	}
	// Updates the current Enemies on  field to the current number
	SetEnemiesLeftOnField();

	// If the player is dead or an enemy reaches the end, the player loses
	if (bIsPlayerDead() || bEnemyHitTrigger)
	{
		bIsGameOver = true;
		EndGame();
	}
	// If there are no enemies left on the field, there are no more waves and no enemies reached the end,
	// the player wins
	if ((EnemiesLeftOnField == 0 && TotalWavesLeft == 0) && !bEnemyHitTrigger)
	{
		bIsGameOver = true;
		bPlayerWon = true;
		EndGame();
	}

	// Checks if a new wave can spawn
	if (bCanSpawn && TotalWavesLeft != 0)
	{
		bCanSpawn = false;
		// Loops through all spawnpoints and spawns an enemy
		for (ASpawnPoint* SpawnPoint : SpawnPoints)
		{
			SpawnPoint->SpawnEnemyShip();
			SetEnemiesLeftToSpawn();
		}
		// Resets the timer, removes a wave and starts the timer handle for bCanSpawn
		Timer = SpawnRate;
		TotalWavesLeft--;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpaceInvadersGameMode::CanNowSpawnNewShip, SpawnRate);
	}
}

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

int ASpaceInvadersGameMode::GetTotalWaves()
{
	return TotalWaves;
}

void ASpaceInvadersGameMode::SetShipsKilled()
{
	EnemyShipsKilled++;
}

void ASpaceInvadersGameMode::SetEnemiesLeftToSpawn()
{
	EnemiesLeftToSpawn--;
}

void ASpaceInvadersGameMode::EndGame()
{
	GetWorld()->GetFirstPlayerController()->Pause();
}

void ASpaceInvadersGameMode::CanNowSpawnNewShip()
{
	bCanSpawn = true;
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
