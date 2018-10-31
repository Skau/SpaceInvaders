// Copyright 1998-2017 Epic Games, Inc. All Rights Reserve

#include "SpaceInvadersProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#include "Gamemodes/SpaceInvadersGameMode.h"
#include "Ships/SpaceInvadersPawn.h"
#include "Ships/Enemy.h"
#include "Ships/EnemyBoss.h"

ASpaceInvadersProjectile::ASpaceInvadersProjectile()
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/Meshes/PlayerProjectile.PlayerProjectile"));

	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ASpaceInvadersProjectile::OnHit);
	RootComponent = ProjectileMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 7500.f;
	ProjectileMovement->MaxSpeed = 7500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after .5 seconds by default so you cannot shoot stuff off-screen
	InitialLifeSpan = .5f;

	// Sets the scale of the projectile
	ProjectileMesh->SetRelativeScale3D(FVector(2, 2, 2));
}

void ASpaceInvadersProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());

	// If the actor hit is an enemy
	if	(OtherActor->IsA(AEnemy::StaticClass()))
	{
		// Destroy the enemy
		OtherActor->Destroy();
		
		if (ExplosionSound != nullptr && GameMode->GetbIsSoundEffectsAllowed())
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), 0.6f);
		}

		if (ExplosionEffect != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, OtherActor->GetActorLocation(), GetActorRotation(), FVector(2));
		}

		// Cast to the current gamemode and increment the kill counter
		GameMode->SetShipsKilled();
	}
	else if (OtherActor->IsA(AEnemyBoss::StaticClass()))
	{
		// Destroy the enemy
		auto Boss = Cast<AEnemyBoss>(OtherActor);
		Boss->DecrementHealth();
		Boss->DecrementHealth();

		if (ExplosionSound != nullptr && GameMode->GetbIsSoundEffectsAllowed())
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), 0.6f);
		}

		if (ExplosionEffect != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), FVector(2));
		}
	}
	else if (OtherActor->IsA(ASpaceInvadersPawn::StaticClass()))
	{
		// Destroy the enemy
		auto Player = Cast<ASpaceInvadersPawn>(OtherActor);
		Player->DecrementHealth();
		Player->DecrementHealth();
		if (ExplosionSound != nullptr && GameMode->GetbIsSoundEffectsAllowed())
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), 0.2f);
		}
		if (ExplosionEffect != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, OtherActor->GetActorLocation(), GetActorRotation(), FVector(0.75));
		}
	}

	// Destroys the projectile
	Destroy();
}