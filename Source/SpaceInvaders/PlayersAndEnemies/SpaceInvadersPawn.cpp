// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersPawn.h"
#include "OtherGameActors/SpaceInvadersProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Gamemodes/SpaceInvadersGameMode.h"
#include "WeaponComponent.h"

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

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");

	MoveSpeed = 1000.0f;

	GunOffset = FVector(180.f, 0.f, 0.f);
}

void ASpaceInvadersPawn::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	// Not currently used (the enemy deals with destroying the player on hit)
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

void ASpaceInvadersPawn::BeginPlay()
{
	Super::BeginPlay();	
	GameMode = Cast<ASpaceInvadersGameMode>(GetWorld()->GetAuthGameMode());
}

void ASpaceInvadersPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Move(DeltaSeconds);
	
	if (GameMode && Health <= 0)
	{
		GameMode->SetPlayerDead(true);
		Destroy();
	}

	if (WeaponComponent && GetInputAxisValue(FireForwardBinding))
	{
		WeaponComponent->Fire();
	}
}

void ASpaceInvadersPawn::Move(float DeltaSeconds)
{
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	const FVector MoveDirection = FVector(0.f, RightValue, 0.f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

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
}

void ASpaceInvadersPawn::PauseGame()
{
	auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
	GameMode->SetGameIsPaused();
}

