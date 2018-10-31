// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersPawn.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"

#include "Gamemodes/SpaceInvadersGameMode.h"
#include "Components/ShipWeaponComponent.h"
#include "Components/ShipMovementComponent.h"
#include "Enemy.h"

const FName ASpaceInvadersPawn::MoveRightBinding("MoveRight");
const FName ASpaceInvadersPawn::FireForwardBinding("FireForward");

ASpaceInvadersPawn::ASpaceInvadersPawn()
{	
	OnActorHit.AddDynamic(this, &ASpaceInvadersPawn::OnHit);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Meshes/PlayerShip/PlayerShip.PlayerShip"));

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);

	WeaponComponent = CreateDefaultSubobject<UShipWeaponComponent>("ShipWeaponComponent");

	MovementComponent = CreateDefaultSubobject<UShipMovementComponent>("ShipMovementComponent");

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

	if (GameMode && Health <= 0)
	{
		GameMode->SetPlayerDead(true);
		Destroy();
	}

	if (MovementComponent)
	{
		MovementComponent->Move(DeltaSeconds, GetInputAxisValue(MoveRightBinding));
	}

	if (WeaponComponent && GetInputAxisValue(FireForwardBinding))
	{
		WeaponComponent->Fire();
	}
}

void ASpaceInvadersPawn::PauseGame()
{
	auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
	GameMode->SetGameIsPaused();
}

