// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "HighscoreSaver.h"
#include "SpaceInvadersGameInstance.h"
#include "OtherGameActors/SpawnPoint.h"

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
			if (!EnemiesLeftOnField)
			{
				SpawnNewBossWave();
				SpawnRate = 3;
				Timer = SpawnRate;
				GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEndlessGameMode::CanNowSpawnNewShip, SpawnRate);
			}
		}
	}
}

void AEndlessGameMode::WinCheck()
{
	if (bBossIsDead)
	{
		for (ASpawnPoint* SpawnPoint : SpawnPoints)
		{
			if (SpawnPoint->GetIfBossSpawnpoint())
			{
				SpawnPoint->SetHaveSpawnedBoss(false);
			}
		}
		EnemyBoss = nullptr;
	}

	// If the player is dead or an enemy reaches the end, the player loses
	if (bPlayerIsDead || bEnemyHitTrigger)
	{
		// Sets that the game is over, and pauses the game (so you can still use UI)
		bIsGameOver = true;
		GetWorld()->GetFirstPlayerController()->Pause();

		// Sets a default name if the player didn't make one
		if (GameInstance->GetPlayerName().Len() == 0)
		{
			GameInstance->SetPlayerName("Player");
		}

		FHighScoreDataGM HighScoreData;
		// Temporary datastruct to add the last game played to the Highscore array
		HighScoreData.PlayerName = GameInstance->GetPlayerName();
		HighScoreData.EnemiesKilled = EnemyShipsKilled;
		HighScoreData.WaveReached = CurrentWave;
		HighScoreData.BossesKilled = BossKills;
		HighScores.Add(HighScoreData);
		SaveData(HighScoreData);
	}
}

void AEndlessGameMode::SaveData(FHighScoreDataGM data)
{
	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::LoadGameFromSlot(LoadedGameObject->SaveSlotName, LoadedGameObject->UserIndex));
	if (!LoadedGameObject)
	{
		LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	}
	auto Data = LoadedGameObject->HighScoreInfo;

	Data.PlayerName = data.PlayerName;
	Data.BossesKilled = data.BossesKilled;
	Data.WaveReached = data.WaveReached;
	Data.EnemiesKilled = data.EnemiesKilled;

	LoadedGameObject->AddDataToArray(Data);

	UGameplayStatics::SaveGameToSlot(LoadedGameObject, LoadedGameObject->SaveSlotName, LoadedGameObject->UserIndex);
}
