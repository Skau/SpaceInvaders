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

#include "Gamemodes/SpaceInvadersGameMode.h"
#include "Ships/SpaceInvadersPawn.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Meshes/AlienShip/Alien.Alien"));

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);
	ShipMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);

	bHitPlayer = false;

	// Set in derived class
	MoveSpeed = 0.0f;
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// If the actor hit is the player, destroy it (ends the game, win check in the gamemode)
	if (OtherActor->IsA(ASpaceInvadersPawn::StaticClass()))
	{
		bHitPlayer = true;
		auto GameMode = Cast<ASpaceInvadersGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->SetPlayerDead(true);
		OtherActor->Destroy();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


