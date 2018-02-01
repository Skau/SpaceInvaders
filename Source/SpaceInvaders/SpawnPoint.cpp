// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h" 
#include "Enemy.h"
#include "EnemyBoss.h"


ASpawnPoint::ASpawnPoint()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> EnemyBP(TEXT("Blueprint'/Game/Blueprints/Enemy_BP.Enemy_BP'"));
	if (EnemyBP.Object)
	{
		Enemy_BP = (UClass*)EnemyBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> EnemyBossBP(TEXT("Blueprint'/Game/Blueprints/EnemyBoss_BP.EnemyBoss_BP'"));
	if (EnemyBossBP.Object)
	{
		EnemyBoss_BP = (UClass*)EnemyBossBP.Object->GeneratedClass;
	}
}

void ASpawnPoint::SpawnEnemyShip()
{
	if (!bIsBossSP)
	{
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Enemy_BP, this->GetActorLocation(), FRotator(0, 0, -180));
	}
	else if (!bHaveSpawnedBoss)
	{
		AEnemyBoss* EnemyBoss = GetWorld()->SpawnActor<AEnemyBoss>(EnemyBoss_BP, this->GetActorLocation(), FRotator(0, 0, -180));
		bHaveSpawnedBoss = true;
	}
}

