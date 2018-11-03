// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighscoreSaver.generated.h"


USTRUCT()
struct FHighScoreInfo 
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString Name;
	UPROPERTY(VisibleAnywhere)
	int WaveReached;
	UPROPERTY(VisibleAnywhere)
	int EnemiesKilled;
	UPROPERTY(VisibleAnywhere)
	int BossKills;
};

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API UHighscoreSaver : public USaveGame
{
	GENERATED_BODY()

public:
	UHighscoreSaver();

	TArray<FHighScoreInfo>& GetHighScoreData() { return HighScoreArray; }

	int AddDataToArray(FHighScoreInfo DataToAdd) { return HighScoreArray.Add(DataToAdd); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UserIndex;

	UPROPERTY(VisibleAnywhere)
	FHighScoreInfo HighScoreInfo; 

	UPROPERTY(VisibleAnywhere)
	TArray<FHighScoreInfo> HighScoreArray;
};
