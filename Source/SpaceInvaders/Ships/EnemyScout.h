// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemyScout.generated.h"

UCLASS()
class SPACEINVADERS_API AEnemyScout : public AEnemy
{
	GENERATED_BODY()
	
public:
protected:	
	// Sets default values for this actor's properties
	AEnemyScout();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Move(float DeltaTime);

	void MoveLeftorRight(float DeltaTime);

	void SetbCanMoveLeftOrRight();

	bool bCanMoveLeftOrRight;

	bool Direction;

	int MoveLeftOrRightRate;

	FTimerHandle MoveLeftOrRightHandle;
};
