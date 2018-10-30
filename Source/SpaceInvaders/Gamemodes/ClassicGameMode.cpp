// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassicGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"

AClassicGameMode::AClassicGameMode(const FObjectInitializer& ObjectInitializer)
	: ASpaceInvadersGameMode(ObjectInitializer)
{
	TotalWaves = 10;
	bIsClassic = true;
}

void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void AClassicGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AClassicGameMode::CanNowSpawnNewShip, SpawnRate);
	}

	// if next wave is the boss wave, only spawn the boss
	else if (bCanSpawn && bIsBossWave)
	{
		if (!EnemiesLeftOnField)
		{
			SpawnNewBossWave();
		}
	}
}

void AClassicGameMode::WinCheck()
{
	// If the player is dead or an enemy reaches the end, the player loses
	if (GetIsPlayerDead() || bEnemyHitTrigger)
	{
		bIsGameOver = true;
		GetWorld()->GetFirstPlayerController()->Pause();
	}

	// the classic checks (no extra checks for endless, as you can never win)
	if (bIsClassic)
	{
		// If there are no enemies left on the field, there are no more waves and no enemies reached the end
		// and the boss is dead, the player wins
		if (EnemiesLeftOnField == 0 && bBossIsDead && !bEnemyHitTrigger)
		{
			bIsGameOver = true;
			bPlayerWon = true;
			GetWorld()->GetFirstPlayerController()->Pause();
		}
	}
}