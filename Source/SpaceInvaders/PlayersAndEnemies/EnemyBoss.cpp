// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBoss.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SpaceInvadersPawn.h"
#include "Gamemodes/SpaceInvadersGameMode.h"
#include "OtherGameActors/SpaceInvadersProjectile.h"

// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Meshes/BossShip/Boss.Boss"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	MeshComponent->SetStaticMesh(Mesh.Object);
	MeshComponent->SetNotifyRigidBodyCollision(true);

	Direction = true;
	FireRate = 0.05;
	TempRate = 3;
	temp = 0;
	RotationOffset = -25;
	bCanFire = true;
	bTempBreak = true;
}

// Called when the game starts or when spawned
void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
	GameMode->SetBossHasSpawned();
	GameMode->SetEnemyBoss(this);
}

// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ShootAWave();

	if (Health <= 0)
	{
		Destroy();
	}

}
// A few ifs here, to control the fire rate and direction
void AEnemyBoss::ShootAWave()
{
	// There are two timerhandles, one for the millisecond between each shot and one is for the 3 second break when it reaches the other corner
	if (bTempBreak)
	{
	
		if (bCanFire)
		{
			// Sets the rotation offset to get the wave movement
			Shoot(RotationOffset);

			// The ifs here control the direction, so it doesn't start at the same corner every time, but alternates instead
			if (Direction)
			{
				if (RotationOffset <= 25)
				{
					RotationOffset++;
				}
				else
				{
					RotationOffset = 25;
					GetWorld()->GetTimerManager().SetTimer(TempHandler, this, &AEnemyBoss::TempTimerExpired, TempRate);
					bTempBreak = false;
					Direction = false;
				}
			}
			else
			{
				if (RotationOffset >= -25)
				{
					RotationOffset--;
				}
				else
				{
					RotationOffset = -25;
					GetWorld()->GetTimerManager().SetTimer(TempHandler, this, &AEnemyBoss::TempTimerExpired, TempRate);
					bTempBreak = false;
					Direction = true;
				}
			}
		}
	}
}
// Main shoot function
void AEnemyBoss::Shoot(int RotationOffset)
{

	if (Projectile_BP != nullptr)
	{
		// There are 3 sockets on the mesh that are used as the locations to shoot from
		TArray<FName> SocketLocations = MeshComponent->GetAllSocketNames();

		// Loops through the sockets to spawn a projectile there
		for (FName& Socket : SocketLocations)
		{
			GetWorld()->SpawnActor<ASpaceInvadersProjectile>(
				Projectile_BP,
				MeshComponent->GetSocketLocation(Socket),
				MeshComponent->GetSocketRotation(Socket) + FRotator(0, RotationOffset, 0)
				);

			GetWorld()->GetTimerManager().SetTimer(TH_ShotTimerExpired, this, &AEnemyBoss::ShotTimerExpired, FireRate);
			bCanFire = false;
		}
	}
}

// Timerhandle bool
void AEnemyBoss::TempTimerExpired()
{
	bTempBreak = true;
}
// Timerhandle bool
void AEnemyBoss::ShotTimerExpired()
{
	bCanFire = true;
}

void AEnemyBoss::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// If the actor hit is the player, destroy it (ends the game, win check in the gamemode)
	if (OtherActor->IsA(ASpaceInvadersPawn::StaticClass()))
	{
		bHitPlayer = true;
		auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
		GameMode->SetPlayerDead(true);
		OtherActor->Destroy();
	}
}
