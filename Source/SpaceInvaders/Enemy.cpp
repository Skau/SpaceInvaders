// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvadersGameMode.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Meshes/EnemyMesh.EnemyMesh"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);
	ShipMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);

}

void AEnemy::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Get player pawn reference
	auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (OtherActor == PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s killed the player!"), *this->GetName())
		auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
		GameMode->EndGame();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

}

void AEnemy::Move(float DeltaTime)
{
	FVector CurrentLocation = this->GetActorLocation();
	FVector MoveDirection = FVector(FVector(-1.f, 0.f, 0.f));
	FVector NewLocation = CurrentLocation + MoveDirection * MoveSpeed * DeltaTime;

	this->SetActorLocation(NewLocation);
}

