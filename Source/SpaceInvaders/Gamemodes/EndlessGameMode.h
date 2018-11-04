// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvadersGameMode.h"
#include "EndlessGameMode.generated.h"

// Forward declarations
struct FHighScoreInfo;

USTRUCT(BlueprintType)
struct FHighScoreDataGM
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WaveReached;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemiesKilled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BossesKilled;
};

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AEndlessGameMode : public ASpaceInvadersGameMode
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	int GetCurrentWave() { return CurrentWave; }

private:
	AEndlessGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	void WinCheck();
	
	void SaveData(FHighScoreInfo data);

	int CurrentWaveCounter;
};
