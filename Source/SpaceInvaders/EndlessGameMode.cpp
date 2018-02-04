// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "HighscoreSaver.h"
#include "SpaceInvadersGameInstance.h"
#include "SpawnPoint.h"

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
	if (bIsPlayerDead() || bEnemyHitTrigger)
	{
		// Sets that the game is over, and pauses the game (so you can still use UI)
		bIsGameOver = true;
		GetWorld()->GetFirstPlayerController()->Pause();

		// Sets a default name if the player didn't make one
		if (GameInstance->GetPlayerName().Len() == 0)
		{
			GameInstance->SetPlayerName("Player");
		}

		// Temporary datastruct to add the last game played to the Highscore array
		FHighScoreDataGM temp;
		temp.PlayerName = GameInstance->GetPlayerName();
		temp.EnemiesKilled = EnemyShipsKilled;
		temp.WaveReached = CurrentWave;
		temp.BossesKilled = BossKills;
		HighScores.Add(temp);

		SaveData();
	}
}

void AEndlessGameMode::SaveData()
{
		//*** SAVING STARTED ***///
	UE_LOG(LogTemp, Warning, TEXT("SAVING STARTED"))

	// Loading data from the file
	UE_LOG(LogTemp, Warning, TEXT("Checking to see if a file is present"))
	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::LoadGameFromSlot(LoadedGameObject->SaveSlotName, LoadedGameObject->UserIndex));
	if (LoadedGameObject != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found file on disk"))
		UE_LOG(LogTemp, Warning, TEXT("Check if data is available in LoadedGameObject"))
		if (LoadedGameObject->GetHighScoreData().Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Data present"))
			UE_LOG(LogTemp, Warning, TEXT("Adding data to temp array"))
			auto DataArray = LoadedGameObject->GetHighScoreData();
			int temp = 0;
			FHighScoreDataGM HighScoreData;
			UE_LOG(LogTemp, Warning, TEXT("Going through saved data and adding it to local highscore"))
			// Go through it all and add to Highscores (used for the in-game table)
			for (auto& Data : DataArray)
			{
				UE_LOG(LogTemp, Warning, TEXT("DataArray index %d"), temp)
				Data.PlayerName = HighScoreData.PlayerName;
				Data.EnemiesKilled = HighScoreData.EnemiesKilled;
				Data.WaveReached = HighScoreData.WaveReached;
				Data.BossesKilled = HighScoreData.BossesKilled;
				HighScores.Add(HighScoreData);
				temp++;
			}

			UE_LOG(LogTemp, Warning, TEXT("Sorting local highscores, ready to be added to savefile"))
			// Sorts the array, so it's ready to be saved
			HighScores.Sort([](const FHighScoreDataGM& Data1, const FHighScoreDataGM& Data2) {
			return Data1.EnemiesKilled > Data2.EnemiesKilled;
			});

			// Emptying array data on file
			UE_LOG(LogTemp, Warning, TEXT("Clearing saved data"))
			LoadedGameObject->HighScoreArray.Empty();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadedGameObject == nullptr, so creating new object to save to"))
		LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	}

	// Adding local highscore to the file array
	UE_LOG(LogTemp, Warning, TEXT("Adding local highscore to file"))
	int temp = 0;
	for (auto& Data : HighScores)
	{
		UE_LOG(LogTemp, Warning, TEXT("HighScores index %d"), temp)
		LoadedGameObject->HighScoreInfo.PlayerName = Data.PlayerName;
		LoadedGameObject->HighScoreInfo.EnemiesKilled = Data.EnemiesKilled;
		LoadedGameObject->HighScoreInfo.WaveReached = Data.WaveReached;
		LoadedGameObject->HighScoreInfo.BossesKilled = Data.BossesKilled;
		LoadedGameObject->AddDataToArray(LoadedGameObject->HighScoreInfo);
		temp++;
	}
	UE_LOG(LogTemp, Warning, TEXT("Saving to file"))
	UGameplayStatics::SaveGameToSlot(LoadedGameObject, LoadedGameObject->SaveSlotName, LoadedGameObject->UserIndex);

	UE_LOG(LogTemp, Warning, TEXT("SAVING FINISHED"))
	//*** SAVING FINISHED ***//
}
