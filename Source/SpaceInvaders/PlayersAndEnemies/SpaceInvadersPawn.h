// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceInvadersPawn.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class AEnemy;
class ASpaceInvadersGameMode;
class ASpaceInvadersProjectile;

UCLASS(Blueprintable)
class ASpaceInvadersPawn : public APawn
{
	GENERATED_BODY()

public:
	// Constructor
	ASpaceInvadersPawn();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// For use when colliding with actors
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	// Offset from the ships location to spawn projectiles 
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	FVector GunOffset;
	
	// How fast the weapon will fire
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	float FireRate;

	// The speed our ship moves around the level
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	float MoveSpeed;

	// Sound to play each time we fire
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	class USoundBase* FireSound;

	// Fire a shot in the specified direction
	void FireShot();

	// Handler for the fire timer expiry
	void ShotTimerExpired();

	// Static names for axis bindings
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;

	// Returns the health for the UI
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; }

	// When the projectile hits, decrement health
	void DecrementHealth() { Health--; }

	// Used to spawn the correct projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpaceInvadersProjectile> Projectile_BP;

private:

	// The mesh component
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(VisibleAnywhere)
	float Health = 100;
	
	// Pointer to the GameMode
	UPROPERTY(VisibleAnywhere)
	ASpaceInvadersGameMode* GameMode;

	// Flag to control firing
	bool bCanFire;

	// Handle for efficient management of ShotTimerExpired timer
	FTimerHandle TimerHandle_ShotTimerExpired;

	void Move(float DeltaSeconds);

	// Pauses the game
	void PauseGame();



};

