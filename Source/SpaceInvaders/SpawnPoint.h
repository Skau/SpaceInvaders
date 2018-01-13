// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SpawnPoint.generated.h"

// Forward declarations
class AEnemy;

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ASpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ASpawnPoint();

	void SpawnEnemyShip();
};
