// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

// Forward declarations
class UHighscoreSaver;
class USoundBase;
class UAudioComponent;
class USpaceInvadersGameInstance;

USTRUCT(BlueprintType)
struct FHighScoreDataMM
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName = "DefaultPlayerName(GameMode)";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WaveReached = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemiesKilled = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BossesKilled = 0;
};

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMainMenuGameMode(const FObjectInitializer& ObjectInitializer);

	// Sets a few variables that cannot be set in the constructor
	virtual void BeginPlay() override;

	// Updates variables, spawns enemies and checks if the game is over
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	bool GetHighScoreIsOpen() { return bHighScoreIsOpen; }

	UFUNCTION(BlueprintCallable)
	void SetHighScoreIsOpen(bool Value) { bHighScoreIsOpen = Value; }
	
	UFUNCTION(BlueprintCallable)
	TArray<FHighScoreDataMM> GetHighScores() { return HighScores; }

	// Used to set the background music
	UPROPERTY(BlueprintReadWrite)
	USoundBase* Music;

	UFUNCTION(BlueprintCallable)
	void LoadHighScore();

	UPROPERTY(BlueprintReadWrite)
	TArray<FHighScoreDataMM> HighScores;

	UHighscoreSaver* LoadedGameObject;

	UAudioComponent* CurrentMusic;

	USpaceInvadersGameInstance* GameInstance;

	bool bHighScoreIsOpen;

	bool bIsMusicAllowed;

	bool bIsCurrentlyPlayingMusic;
};
