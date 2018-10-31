// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyScout.h"
#include "Engine/World.h"
#include "TimerManager.h"

AEnemyScout::AEnemyScout()
{
	PrimaryActorTick.bCanEverTick = true;

	bCanMoveLeftOrRight = true;
	MoveLeftOrRightRate = 2.f;
	MoveSpeed = 150.f;
}

void AEnemyScout::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyScout::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetHasHitPlayer())
	{
		Move(DeltaTime);
	}
	// if the timer is over
	if (bCanMoveLeftOrRight)
	{
		// Sets the timer to end between 1 and 3 seconds (to make it more random when they move)
		bCanMoveLeftOrRight = false;
		MoveLeftOrRightRate = FMath::RandRange(1, 3);
		GetWorld()->GetTimerManager().SetTimer(MoveLeftOrRightHandle, this, &AEnemyScout::SetbCanMoveLeftOrRight, MoveLeftOrRightRate);
		MoveLeftorRight(DeltaTime);
	}
}

void AEnemyScout::Move(float DeltaTime)
{
	FVector NewLocation = GetActorLocation() + FVector(-1.f, 0.f, 0.f) * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation);
}

void AEnemyScout::MoveLeftorRight(float DeltaTime)
{
	FVector MoveDirection = Direction ? FVector(0.f, 350.f, 0.f) : FVector(0.f, -350.f, 0.f);
	Direction = !Direction;
	SetActorLocation(GetActorLocation() + MoveDirection);
}

void AEnemyScout::SetbCanMoveLeftOrRight()
{
	bCanMoveLeftOrRight = true;
}

