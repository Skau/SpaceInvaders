// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "HighScoreSaver.h"
#include "SpaceInvadersGameInstance.h"

AMainMenuGameMode::AMainMenuGameMode(const FObjectInitializer& ObjectInitializer)
{

	CurrentMusic = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("BackgroundMusic"));
	RootComponent = CurrentMusic;
	PrimaryActorTick.bCanEverTick = true;

	LoadHighScore();
}

// Init the game variables that has to be set at runtime
void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Gets the current game instance
	GameInstance = Cast<USpaceInvadersGameInstance>(GetWorld()->GetGameInstance());

	if (Music != nullptr)
	{
		CurrentMusic->SetSound(Music);
		CurrentMusic->bIsUISound = true;

		if (GameInstance->GetMusicAllowed())
		{
			CurrentMusic->Play();
			bIsCurrentlyPlayingMusic = true;
		}

		if (!GameInstance->GetMusicAllowed())
		{
			CurrentMusic->Stop();
			bIsCurrentlyPlayingMusic = false;
		}
	}
}

void AMainMenuGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Music != nullptr)
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
	//*** LOADING HIGHSCORES  ***//
	UE_LOG(LogTemp, Warning, TEXT("LOADING STARTED"))
	// If there's any data in Highscores, clear it
	if (HighScores.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Clearing highscores"))
		HighScores.Empty();
	}

	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::LoadGameFromSlot(LoadedGameObject->SaveSlotName, LoadedGameObject->UserIndex));
	UE_LOG(LogTemp, Warning, TEXT("Checking to see if a file is present"))
	if (LoadedGameObject != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found file on disk"))
	}
	else
	{
		// Create a SaveObject if none are present
		UE_LOG(LogTemp, Warning, TEXT("No file found, creating new"))
		LoadedGameObject = Cast<UHighscoreSaver>(UGameplayStatics::CreateSaveGameObject(UHighscoreSaver::StaticClass()));
	}
	//  Make a temporary Array of the savefile if data is available
	if (LoadedGameObject->GetHighScoreData().Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Data present on save, adding data to local highscore"))
		auto LoadedGameData = LoadedGameObject->GetHighScoreData();

		int temp = 0;
		FHighScoreDataMM Data;
		// Go through it all and add to Highscores (used for the in-game table)
		for (auto& HighScoreData : LoadedGameData)
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding HighscoreData index %d"), temp)
			Data.PlayerName = HighScoreData.PlayerName;
			Data.EnemiesKilled = HighScoreData.EnemiesKilled;
			Data.WaveReached = HighScoreData.WaveReached;
			Data.BossesKilled = HighScoreData.BossesKilled;
			HighScores.Add(Data);
			temp++;
		}
		UE_LOG(LogTemp, Warning, TEXT("Sorting Highscores"))
		HighScores.Sort([](const FHighScoreDataMM& LHS, const FHighScoreDataMM& RHS) { return LHS.EnemiesKilled > RHS.EnemiesKilled; });
		
		UE_LOG(LogTemp, Warning, TEXT("LOADING FINISHED"))
		//*** LOADING FINISHED ***//
	}
}