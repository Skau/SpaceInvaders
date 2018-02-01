// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBoss.generated.h"

// Forward declarations
class ASpaceInvadersProjectile;

UCLASS()
class SPACEINVADERS_API AEnemyBoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBoss();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Main function to control the shooting
	void ShootAWave();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 25.f;

	// Used everytime the ship is hit
	void DecrementHealth() { Health--; }

	// Used by the UI
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; }

	// For spawning the correct projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpaceInvadersProjectile> Projectile_BP;

private:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	float Health = 100;

	void Shoot(int RotationOffset);

	int RotationOffset;

	int temp;

	bool Direction;

	bool bTempBreak;

	bool bCanFire;

	bool bHitPlayer = false;

	void TempTimerExpired();

	void ShotTimerExpired();
	
	float TempRate;

	float FireRate;

	FTimerHandle TempHandler;

	FTimerHandle TH_ShotTimerExpired;
};
