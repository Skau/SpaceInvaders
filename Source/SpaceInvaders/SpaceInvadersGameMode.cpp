// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersPawn.h"
#include "Public/EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "SpawnPoint.h"

ASpaceInvadersGameMode::ASpaceInvadersGameMode()
{
	// set default pawn class to our character class
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
	TotalWavesLeft = 5;
	SpawnRate = 5.f;
	EnemiesLeftToSpawn = 8 * TotalWavesLeft;
}

void ASpaceInvadersGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASpawnPoint> It(GetWorld(), ASpawnPoint::StaticClass()); It; ++It)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(*It);
		SpawnPoints.Add(SpawnPoint);
	}
}

void ASpaceInvadersGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bCanSpawn)
	{
		Timer = Timer - DeltaSeconds*1;
	}

	SetEnemiesLeftOnField();

	if (bIsPlayerDead() || bEnemyHitTrigger)
	{
		bIsGameOver = true;
		EndGame();
	}

	if ((EnemiesLeftOnField == 0 && TotalWavesLeft == 0) && !bEnemyHitTrigger)
	{
		bIsGameOver = true;
		bPlayerWon = true;
		EndGame();
	}

	if (bCanSpawn && TotalWavesLeft != 0)
	{
		bCanSpawn = false;
		for (ASpawnPoint* SpawnPoint : SpawnPoints)
		{
			SpawnPoint->SpawnEnemyShip();
			SetEnemiesLeftToSpawn();
		}
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

void ASpaceInvadersGameMode::SetEnemiesLeftOnField()
{
	int temp = 0;

	for (TActorIterator<AEnemy> It(GetWorld(), AEnemy::StaticClass()); It; ++It)
	{
		temp++;
	}

	EnemiesLeftOnField = temp;
}
