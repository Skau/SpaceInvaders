// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipDestroyTrigger.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "SpaceInvadersGameMode.h"
#include "Enemy.h"

// Sets default values
AShipDestroyTrigger::AShipDestroyTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (TriggerVolume == nullptr) { return; }

	RootComponent = TriggerVolume;
}

// Called when the game starts or when spawned
void AShipDestroyTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AShipDestroyTrigger::OnBeginOverlap);
}

// Called every frame
void AShipDestroyTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Destroys the enemy if it reaches it, and ends the game
void AShipDestroyTrigger::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AEnemy::StaticClass()))
	{
		AActor* Enemy(Cast<AEnemy>(OtherActor));
		if (Enemy)
		{
			Enemy->Destroy();
			auto GameMode = (ASpaceInvadersGameMode*)(GetWorld()->GetAuthGameMode());
			GameMode->bEnemyHitTrigger = true;
		}
	}
}

