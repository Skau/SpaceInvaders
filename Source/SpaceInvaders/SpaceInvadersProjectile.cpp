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
#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersPawn.h"
#include "Enemy.h"

//{
//
//}

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

	// Die after 3 seconds by default
	InitialLifeSpan = 0.75f;

	// Sets the scale of the projectile
	ProjectileMesh->SetRelativeScale3D(FVector(2, 2, 2));
}

void ASpaceInvadersProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If the actor hit is an enemy
	if	(OtherActor->IsA(AEnemy::StaticClass()))
	{
		// Destroy the enemy
		OtherActor->Destroy();
		
		// Cast to the current gamemode and increment the kill counter
		auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
		GameMode->SetShipsKilled();
	}

	// Destroys the projectile
	Destroy();
}