// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "CoreMisc.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Misc/FileHelper.h"

#include "Other/HighscoreSaver.h"
#include "SpaceInvadersGameInstance.h"
#include "Actors/SpawnPoint.h"

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
	FString FolderName = "Highscore";
	FString FileName = "SaveFile.txt";
	FString JsonString = "";
	auto Directory = FPaths::ProjectDir() + "/" + FolderName;
	FString AbsolutePath = Directory + "/" + FileName;
	TArray<TSharedPtr<FJsonValue>> ObjArray;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();


	// If the directory is found
	if (PlatformFile.DirectoryExists(*Directory))
	{
		// Load current data in text file if exists
		if (FFileHelper::LoadFileToString(JsonString, *AbsolutePath))
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
			TSharedPtr<FJsonObject> CurrentDataJsonObject = MakeShareable(new FJsonObject);
			if (FJsonSerializer::Deserialize(Reader, CurrentDataJsonObject))
			{
				// Get array 
				ObjArray = CurrentDataJsonObject->GetArrayField("Players");
			}
		}
	}
		
	// Create a JsonObject and add the data
	TSharedPtr<FJsonObject> NewPlayerDataJsonObject = MakeShareable(new FJsonObject);
	NewPlayerDataJsonObject->SetStringField("Name", data.PlayerName);
	NewPlayerDataJsonObject->SetNumberField("BossKills", data.BossesKilled);
	NewPlayerDataJsonObject->SetNumberField("WaveReached", data.WaveReached);
	NewPlayerDataJsonObject->SetNumberField("EnemiesKilled", data.EnemiesKilled);
	TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(NewPlayerDataJsonObject));
	ObjArray.Add(JsonValue);

	TSharedPtr<FJsonObject> DataToSaveJsonObject = MakeShareable(new FJsonObject);
	DataToSaveJsonObject->SetArrayField("Players", ObjArray);

	// Serialize the data to FString
	FString outputString;
	TSharedRef <TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&outputString);

	// If successful, write the string to a text file
	if (FJsonSerializer::Serialize(DataToSaveJsonObject.ToSharedRef(), JsonWriter))
	{
		// Get reference to file manager
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		auto Directory = FPaths::ProjectDir() + "/" + FolderName;

		FString AbsolutePath = Directory + "/" + FileName;

		// Save to file
		if (FFileHelper::SaveStringToFile(outputString, *AbsolutePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("Sucessfully wrote to file"))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed writing to file"))
		}
	}
}
