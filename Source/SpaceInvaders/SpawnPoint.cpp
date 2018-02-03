// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "Engine/World.h" 
#include "Enemy.h"
#include "EnemyBoss.h"

// Not currently in use
ASpawnPoint::ASpawnPoint()
{
}


// Spawns the correct enemy
void ASpawnPoint::SpawnEnemyShip()
{
	// If the bool isn't checked
	if (!bIsBossSP && Enemy_BP != nullptr)
	{
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Enemy_BP, this->GetActorLocation(), FRotator(0, 0, -180));
	}
	// Only do this if the boss haven't spawned already to stop multiple bosses from spawning (the gamemode takes care of when the boss wave is)
	else if (!bHaveSpawnedBoss && EnemyBoss_BP != nullptr)
	{
		AEnemyBoss* EnemyBoss = GetWorld()->SpawnActor<AEnemyBoss>(EnemyBoss_BP, this->GetActorLocation(), FRotator(0, 0, -180));
		bHaveSpawnedBoss = true;
	}
}

