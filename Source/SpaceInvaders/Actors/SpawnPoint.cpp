// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "Engine/World.h" 

#include "Ships/Enemy.h"
#include "Ships/EnemyBoss.h"


ASpawnPoint::ASpawnPoint()
{
}

void ASpawnPoint::SpawnEnemyShip()
{
	if (Enemy_BP && !bIsBossSP)
	{
		GetWorld()->SpawnActor<AEnemy>(Enemy_BP, GetActorLocation(), FRotator(0, 0, -180));
	}

	else if (EnemyBoss_BP && !bHaveSpawnedBoss)
	{
		GetWorld()->SpawnActor<AEnemyBoss>(EnemyBoss_BP, GetActorLocation(), FRotator(0, 0, -180));
		bHaveSpawnedBoss = true;
	}
}

