// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

// Forward Declarations
class ASpaceInvadersGameMode;

UCLASS()
class SPACEINVADERS_API AEnemy : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void Move(float DeltaTime);

	void MoveLeftorRight(float DeltaTime);

	void SetbCanMoveLeftOrRight();

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Gameplay, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 150.f;

	bool bHitPlayer;

	bool bCanMoveLeftOrRight;

	bool Direction;

	int MoveLeftOrRightRate;

	FTimerHandle MoveLeftOrRightHandle;
};
