// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Enemy.h"


ASpawnPoint::ASpawnPoint()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> EnemyBP(TEXT("Blueprint'/Game/Blueprints/Enemy_BP.Enemy_BP'"));
	if (EnemyBP.Object)
	{
		Enemy_BP = (UClass*)EnemyBP.Object->GeneratedClass;
	}
}

void ASpawnPoint::SpawnEnemyShip()
{
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Enemy_BP, this->GetActorLocation(), FRotator(0, 0, -180));
}

