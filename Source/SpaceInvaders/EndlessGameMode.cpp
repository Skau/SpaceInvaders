// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"

AEndlessGameMode::AEndlessGameMode(const FObjectInitializer& ObjectInitializer)
	: ASpaceInvadersGameMode(ObjectInitializer)
{
	TotalWaves = 9999;
	CurrentWaveCounter = 0;
	CurrentWave = 0;
	bIsEndless = true;
}
void AEndlessGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AEndlessGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WinCheck();

	if (!bBossHasSpawned)
	{
		// Spawns a new wave if conditions allow
		if (bCanSpawn && !bIsBossWave)
		{
			bCanSpawn = false;
			SpawnNewWave();
			CurrentWaveCounter++;
			CurrentWave++;
			Timer = SpawnRate;
			if (CurrentWaveCounter == 10)
			{
				bIsBossWave = true;
				CurrentWaveCounter = 0;
				SpawnRate = 11;
				Timer = SpawnRate;
			}
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEndlessGameMode::CanNowSpawnNewShip, SpawnRate);
		}

		// if next wave is the boss wave, only spawn the boss
		else if (bCanSpawn && bIsBossWave)
		{
			SpawnNewBossWave();
			SpawnRate = 3;
			Timer = SpawnRate;
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEndlessGameMode::CanNowSpawnNewShip, SpawnRate);
		}
	}
}
