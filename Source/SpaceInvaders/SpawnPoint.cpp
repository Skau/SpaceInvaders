// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Enemy.h"

ASpawnPoint::ASpawnPoint()
{
}

void ASpawnPoint::SpawnEnemyShip()
{
	AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(this->GetActorLocation(), FRotator(1, 0, 0));
}

