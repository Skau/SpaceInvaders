// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBoss.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SpaceInvadersPawn.h"
#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersProjectile.h"

// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Meshes/BossShip/Boss.Boss"));
	// Create the mesh component
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

void AEnemyBoss::ShootAWave()
{
	if (bTempBreak)
	{
		if (bCanFire)
		{
			Shoot(RotationOffset);

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

void AEnemyBoss::Shoot(int RotationOffset)
{

	if (Projectile_BP != nullptr)
	{
		TArray<FName> SocketLocations = MeshComponent->GetAllSocketNames();

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

void AEnemyBoss::TempTimerExpired()
{
	bTempBreak = true;
}

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
		GameMode->bPlayerIsDead = true;
		OtherActor->Destroy();
	}
}
