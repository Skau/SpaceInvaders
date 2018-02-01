// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBoss.generated.h"

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

	void ShootAWave();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* The speed the ship */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 25.f;

	void DecrementHealth() { Health--; }

	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; }

private:
	// The mesh component
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	void Shoot(int RotationOffset);

	int RotationOffset;

	int temp;

	bool Direction;

	bool bTempBreak;

	bool bCanFire;

	void TempTimerExpired();

	void ShotTimerExpired();
	
	float TempRate;

	float FireRate;
	FTimerHandle TempHandler;

	FTimerHandle TH_ShotTimerExpired;

	UPROPERTY(VisibleAnywhere)
	float Health = 100;

	bool bHitPlayer = false;
};
