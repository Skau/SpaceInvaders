// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceInvadersPawn.generated.h"

// Forward Declarations
class AEnemy;
class ASpaceInvadersGameMode;
class ASpaceInvadersProjectile;

UCLASS(Blueprintable)
class ASpaceInvadersPawn : public APawn
{
	GENERATED_BODY()

	// The mesh component
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

public:
	// Constructor
	ASpaceInvadersPawn();

	// For use when colliding with actors
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	// Offset from the ships location to spawn projectiles 
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
	FVector GunOffset;
	
	// How fast the weapon will fire
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	// The speed our ship moves around the level
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	// Sound to play each time we fire
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	// Fire a shot in the specified direction
	void FireShot(FVector FireDirection);

	// Handler for the fire timer expiry
	void ShotTimerExpired();

	// Static names for axis bindings
	static const FName MoveForwardBinding; // not currently used
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;

	// Returns ShipMeshComponent subobject
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }

	// Returns the health for the UI
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; }

	// When the projectile hits, decrement health
	void DecrementHealth() { Health--; }

	// Used to spawn the correct projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpaceInvadersProjectile> Projectile_BP;

private:
	UPROPERTY(VisibleAnywhere)
	float Health = 100;

	// Flag to control firing
	uint32 bCanFire : 1;

	// Handle for efficient management of ShotTimerExpired timer
	FTimerHandle TimerHandle_ShotTimerExpired;

	// Pauses the game
	void PauseGame();

	ASpaceInvadersGameMode* GameMode;

};

