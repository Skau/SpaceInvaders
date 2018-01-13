// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Enemy.h"

void ASpawnPoint::SpawnEnemyShip()
{
	GetWorld()->SpawnActor<AEnemy>(this->GetActorLocation(), FRotator(1, 0, 0));
}

