// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceInvadersGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API USpaceInvadersGameInstance : public UGameInstance
{
	GENERATED_BODY()


	// Used for the settings menu
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FString Name) { PlayerName = Name; }

	UFUNCTION(BlueprintCallable)
	int GetTotalWaves() { return TotalWaves; }

	UFUNCTION(BlueprintCallable)
	void SetTotalWaves(int NewTotalWaves) { TotalWaves = NewTotalWaves; }

	UFUNCTION(BlueprintCallable)
	int GetSpawnRate() { return SpawnRate; }

	UFUNCTION(BlueprintCallable)
	void SetSpawnRate(int NewSpawnRate) { SpawnRate = NewSpawnRate; }

	UFUNCTION(BlueprintCallable)
	bool GetMusicAllowed() { return bIsMusicAllowed; }

	UFUNCTION(BlueprintCallable)
	void SetMusicAllowed(bool value) { bIsMusicAllowed = value; }

	UFUNCTION(BlueprintCallable)
	bool GetSoundEffectsAllowed() { return bIsSoundEffectsAllowed; }

	UFUNCTION(BlueprintCallable)
	void SetSoundEffectsAllowed(bool value) { bIsSoundEffectsAllowed = value; }

	FString GetPlayerName() { return PlayerName; }

private:
	bool bHighScoreIsLoaded;
	bool bIsMusicAllowed = true;
	bool bIsSoundEffectsAllowed = true;
	int TotalWaves = 10;
	int SpawnRate = 3;
	FString PlayerName;
	
	
};
