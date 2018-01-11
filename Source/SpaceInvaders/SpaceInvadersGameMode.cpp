// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersPawn.h"
#include "Public/EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "SpawnPoint.h"

ASpaceInvadersGameMode::ASpaceInvadersGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ASpaceInvadersPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	bCanSpawn = true;
	bPlayerHitByEnemy = false;
	TotalEnemyShips = 0;
}

void ASpaceInvadersGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASpawnPoint> It(GetWorld(), ASpawnPoint::StaticClass()); It; ++It)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(*It);
		SpawnPoints.Add(SpawnPoint);
	}
}

void ASpaceInvadersGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetTotalEnemyShips();

	if (bCanSpawn)
	{
		bCanSpawn = false;
		for (ASpawnPoint* SpawnPoint : SpawnPoints)
		{
			SpawnPoint->SpawnEnemyShip();
		}
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpaceInvadersGameMode::CanNowSpawnNewShip, SpawnRate);
	}
}

int ASpaceInvadersGameMode::GetTotalEnemyShips()
{
	return TotalEnemyShips;
}

void ASpaceInvadersGameMode::SetTotalEnemyShips()
{
	int temp = 0;
	
	for (TActorIterator<AEnemy> It(GetWorld(), AEnemy::StaticClass()); It; ++It)
	{
		temp++;
	}

	TotalEnemyShips = temp;

}

void ASpaceInvadersGameMode::EndGame()
{
	GetWorld()->GetFirstPlayerController()->Pause();
}

void ASpaceInvadersGameMode::CanNowSpawnNewShip()
{
	bCanSpawn = true;
}
