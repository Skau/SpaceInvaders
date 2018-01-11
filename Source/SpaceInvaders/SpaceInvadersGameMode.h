// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceInvadersGameMode.generated.h"


// Forward declarations
class AEnemy;
class ASpawnPoint;

UCLASS(MinimalAPI)
class ASpaceInvadersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaceInvadersGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	int GetTotalEnemyShips();

	void EndGame();

private:

	void CanNowSpawnNewShip();

	void SetTotalEnemyShips();

	int TotalEnemyShips;

	bool bCanSpawn;

	bool bPlayerHitByEnemy;

	float SpawnRate = 3.f;

	FTimerHandle SpawnTimerHandle;

	TArray<ASpawnPoint*> SpawnPoints;

};



