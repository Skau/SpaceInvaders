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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Meshes/PlayerMesh.PlayerMesh"));
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
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = .25f;
	bCanFire = true;
}

void ASpaceInvadersPawn::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	// Checks to see if the actor hit is an enemy
	if (OtherActor->IsA(AEnemy::StaticClass()))
	{
		// Redundant cast for now, could probably just use OtherActor
		AActor* Enemy(Cast<AEnemy>(OtherActor));
		if (Enemy)
		{
			ShipMeshComponent->ToggleVisibility(false);
			auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
			GameMode->bPlayerIsDead = true;
		}
	}
}

void ASpaceInvadersPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
}

void ASpaceInvadersPawn::Tick(float DeltaSeconds)
{
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	const FVector MoveDirection = FVector(0.f, RightValue, 0.f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, FRotator(0), true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, FRotator(0), true);
		}
	}
	
	const bool IsFirePressed = GetInputAxisValue(FireForwardBinding);
	if (IsFirePressed)
	{
		// Fire a shot on the x-axis only
		FireShot(FVector(1, 0, 0));
	}
}

void ASpaceInvadersPawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		const FRotator FireRotation = FireDirection.Rotation();
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile
			World->SpawnActor<ASpaceInvadersProjectile>(SpawnLocation, FireRotation);
		}
		bCanFire = false;
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceInvadersPawn::ShotTimerExpired, FireRate);

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		bCanFire = false;
	}
}

void ASpaceInvadersPawn::ShotTimerExpired()
{
	bCanFire = true;
}

