// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceInvadersPawn.generated.h"

class UStaticMeshComponent;
class UShipMovementComponent;
class UShipWeaponComponent;
class AEnemy;
class ASpaceInvadersGameMode;

UCLASS(Blueprintable)
class ASpaceInvadersPawn : public APawn
{
	GENERATED_BODY()

public:
	ASpaceInvadersPawn();

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; }

	void SetHealth(float Value) { Health = Value; }
	void IncrementHealth() { Health++; }
	void DecrementHealth() { Health--; }

	FVector GetGunOffset() { return GunOffset; }

private:
	// -- Functions -- //

	void PauseGame();

	// -- Variables -- //

	ASpaceInvadersGameMode* GameMode;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UShipWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere)
	UShipMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float Health = 100;
	
	UPROPERTY(EditDefaultsOnly)
	FVector GunOffset;

	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
};

