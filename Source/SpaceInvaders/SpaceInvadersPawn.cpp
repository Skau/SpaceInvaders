// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersPawn.h"
#include "SpaceInvadersProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Enemy.h"
#include "SpaceInvadersGameMode.h"

const FName ASpaceInvadersPawn::MoveRightBinding("MoveRight");
const FName ASpaceInvadersPawn::FireForwardBinding("FireForward");

ASpaceInvadersPawn::ASpaceInvadersPawn()
{	
	// To enable on hit events
	OnActorHit.AddDynamic(this, &ASpaceInvadersPawn::OnHit);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Meshes/PlayerShip/PlayerShip.PlayerShip"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/ProjectileSound.ProjectileSound"));
	FireSound = FireAudio.Object;

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(180.f, 0.f, 0.f);
	FireRate = .25f;
	bCanFire = true;
}

// Not currently used (the enemy deals with destroying the player on hit)
void ASpaceInvadersPawn::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
}

void ASpaceInvadersPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// Used the default template way (the variables are const at the top of this file)
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);

	// For the pause menu
	PlayerInputComponent->BindAction("PauseGame", IE_Released, this, &ASpaceInvadersPawn::PauseGame);
}

void ASpaceInvadersPawn::Tick(float DeltaSeconds)
{
	// Variables used for calculating movement
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	const FVector MoveDirection = FVector(0.f, RightValue, 0.f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, FRotator(0), true, &Hit);
		
		// If a wall is hit, stop the player from moving through it
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, FRotator(0), true);
		}
	}
	
	bool IsFirePressed = GetInputAxisValue(FireForwardBinding);
	if (IsFirePressed)
	{
		FireShot(FVector(1, 0, 0));
	}

	if (Health <= 0)
	{
		auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
		GameMode->bPlayerIsDead = true;
		Destroy();
	}
}

void ASpaceInvadersPawn::FireShot(FVector FireDirection)
{
	if (bCanFire == true && Projectile_BP != nullptr)
	{
		FRotator FireRotation = FireDirection.Rotation();
		FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);
	
		GetWorld()->SpawnActor<ASpaceInvadersProjectile>(Projectile_BP, SpawnLocation, FireRotation);
		
		bCanFire = false;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceInvadersPawn::ShotTimerExpired, FireRate);

		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
		}
	}
}

void ASpaceInvadersPawn::ShotTimerExpired()
{
	bCanFire = true;
}

void ASpaceInvadersPawn::PauseGame()
{
	auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
	GameMode->SetGameIsPaused();
}

