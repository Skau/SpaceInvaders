// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "HighScoreSaver.h"
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

	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::LoadGameFromSlot(LoadedGameObject->SaveSlotName, LoadedGameObject->UserIndex));

	if (!LoadedGameObject)
	{
		LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	}

	if (LoadedGameObject->GetHighScoreData().Num())
	{
		auto LoadedGameData = LoadedGameObject->GetHighScoreData();


		FHighScoreDataMM Data;
		for (auto& HighScoreData : LoadedGameData)
		{
			Data.PlayerName = HighScoreData.PlayerName;
			Data.EnemiesKilled = HighScoreData.EnemiesKilled;
			Data.WaveReached = HighScoreData.WaveReached;
			Data.BossesKilled = HighScoreData.BossesKilled;
			HighScores.Add(Data);
		}
		HighScores.Sort([](const FHighScoreDataMM& LHS, const FHighScoreDataMM& RHS) { return LHS.EnemiesKilled > RHS.EnemiesKilled; });
	}
}