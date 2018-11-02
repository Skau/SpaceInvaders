// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

#include "Other/HighScoreSaver.h"
#include "SpaceInvadersGameInstance.h"

AMainMenuGameMode::AMainMenuGameMode(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentMusic = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("BackgroundMusic"));
	RootComponent = CurrentMusic;

	LoadHighScore();
}

// Init the game variables that has to be set at runtime
void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Gets the current game instance
	GameInstance = Cast<USpaceInvadersGameInstance>(GetWorld()->GetGameInstance());

	if (Music)
	{
		CurrentMusic->SetSound(Music);
		CurrentMusic->bIsUISound = true;

		if (GameInstance->GetMusicAllowed())
		{
			CurrentMusic->Play();
			bIsCurrentlyPlayingMusic = true;
		}
		else
		{
			CurrentMusic->Stop();
			bIsCurrentlyPlayingMusic = false;
		}
	}
}

void AMainMenuGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Music)
	{
		if (!GameInstance->GetMusicAllowed() && bIsCurrentlyPlayingMusic)
		{
			CurrentMusic->Stop();
			bIsCurrentlyPlayingMusic = false;
		}

		if (!GameInstance->GetMusicAllowed() && !bIsCurrentlyPlayingMusic)
		{
			CurrentMusic->Play();
			bIsCurrentlyPlayingMusic = true;
		}
	}
}

void AMainMenuGameMode::LoadHighScore()
{
	if (HighScores.Num())
	{
		HighScores.Empty();
	}

	TSharedPtr<FJsonObject> JSonObject = MakeShareable(new FJsonObject);

	FString FolderName = "Highscore";
	FString FileName = "SaveFile.txt";
	FString JsonString = "";

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	auto Directory = FPaths::ProjectDir() + "/" + FolderName;
	
	if (PlatformFile.DirectoryExists(*Directory))
	{
		FString AbsolutePath = Directory + "/" + FileName;
		FFileHelper::LoadFileToString(JsonString, *AbsolutePath);

		if (JsonString.Len())
		{
			FHighScoreDataMM data;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
			if (FJsonSerializer::Deserialize(Reader, JSonObject))
			{
				data.PlayerName = JSonObject->GetStringField("Name");
				data.BossesKilled = JSonObject->GetNumberField("BossKills");
				data.WaveReached = JSonObject->GetNumberField("WaveReached");
				data.EnemiesKilled = JSonObject->GetNumberField("EnemiesKilled");
				HighScores.Add(data);
			}
		}
	}
	HighScores.Sort([](const FHighScoreDataMM& LHS, const FHighScoreDataMM& RHS) { return LHS.EnemiesKilled > RHS.EnemiesKilled; });
}