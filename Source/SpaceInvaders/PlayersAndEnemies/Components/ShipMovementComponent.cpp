// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipMovementComponent.h"
#include "PlayersAndEnemies/SpaceInvadersPawn.h"

UShipMovementComponent::UShipMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MoveSpeed = 1000.0f;
}

void UShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShipMovementComponent::Move(float DeltaTime, float RightValue)
{
	const FVector MoveDirection = FVector(0.f, RightValue, 0.f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

	if (Movement.SizeSquared() > 0.0f)
	{
		FHitResult Hit(1.f);
		GetOwner()->GetRootComponent()->MoveComponent(Movement, FRotator(0), true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			GetOwner()->GetRootComponent()->MoveComponent(Deflection, FRotator(0), true);
		}
	}
}

