// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h" 
#include "Enemy.h"
#include "EnemyBoss.h"


// Finds the correct blueprints (bad way of doing this, but I'm not bothered to change it)
ASpawnPoint::ASpawnPoint()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> EnemyBP(TEXT("Class'/Game/Blueprints/Enemy_BP.Enemy_BP'"));
	if (EnemyBP.Object)
	{
		Enemy_BP = (UClass*)EnemyBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> EnemyBossBP(TEXT("Class'/Game/Blueprints/EnemyBoss_BP.EnemyBoss_BP'"));
	if (EnemyBossBP.Object)
	{
		EnemyBoss_BP = (UClass*)EnemyBossBP.Object->GeneratedClass;
	}
}

// Spawns the correct enemy
void ASpawnPoint::SpawnEnemyShip()
{
	// If the bool isn't checked
	if (!bIsBossSP)
	{
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Enemy_BP, this->GetActorLocation(), FRotator(0, 0, -180));
	}
	// Only do this if the boss haven't spawned already to stop multiple bosses from spawning (the gamemode takes care of when the boss wave is)
	else if (!bHaveSpawnedBoss)
	{
		AEnemyBoss* EnemyBoss = GetWorld()->SpawnActor<AEnemyBoss>(EnemyBoss_BP, this->GetActorLocation(), FRotator(0, 0, -180));
		bHaveSpawnedBoss = true;
	}
}

