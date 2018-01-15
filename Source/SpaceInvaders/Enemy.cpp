// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersPawn.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Meshes/EnemyMesh.EnemyMesh"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);
	ShipMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);

	bCanMoveLeftOrRight = true;
	MoveLeftOrRightRate = 2.f;
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(ASpaceInvadersPawn::StaticClass()))
	{
		AActor* Player(Cast<ASpaceInvadersPawn>(OtherActor));

		if (Player)
		{
			bHitPlayer = true;
			auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
			GameMode->bPlayerIsDead = true;
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHitPlayer)
	{
		Move(DeltaTime);
	}

	if (bCanMoveLeftOrRight)
	{
		bCanMoveLeftOrRight = false;
		MoveLeftOrRightRate = FMath::RandRange(1, 3);
		GetWorld()->GetTimerManager().SetTimer(MoveLeftOrRightHandle, this, &AEnemy::SetbCanMoveLeftOrRight, MoveLeftOrRightRate);
		MoveLeftorRight(DeltaTime);
	}
}

void AEnemy::Move(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector MoveDirection = FVector(-1.f, 0.f, 0.f);
	FVector NewLocation = CurrentLocation + MoveDirection * MoveSpeed * DeltaTime;

	SetActorLocation(NewLocation);
}

void AEnemy::MoveLeftorRight(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation;
	if (Direction)
	{
		FVector MoveDirection = FVector(0.f, 350.f, 0.f);
		NewLocation = CurrentLocation + MoveDirection;
		Direction = false;
	}
	else
	{
		FVector MoveDirection = FVector(0.f, -350.f, 0.f);
		NewLocation = CurrentLocation + MoveDirection;
		Direction = true;
	}
	SetActorLocation(NewLocation);
}

void AEnemy::SetbCanMoveLeftOrRight()
{
	bCanMoveLeftOrRight = true;
}

