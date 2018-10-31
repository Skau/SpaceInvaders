// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipWeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"

#include "Gamemodes/SpaceInvadersGameMode.h"
#include "Ships/SpaceInvadersPawn.h"
#include "Actors/SpaceInvadersProjectile.h"

UShipWeaponComponent::UShipWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/ProjectileSound.ProjectileSound"));
	FireSound = FireAudio.Object;
	FireRate = .25f;
	bCanFire = true;
}

void UShipWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<ASpaceInvadersGameMode>(GetWorld()->GetAuthGameMode());
	Player = Cast<ASpaceInvadersPawn>(GetOwner());
}

void UShipWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShipWeaponComponent::Fire()
{
	if (!Player || !GameMode) { return; }

	if (bCanFire && Projectile_BP)
	{
		FireShot();
	}
}

void UShipWeaponComponent::FireShot()
{
	FVector FireDirection = FVector(1, 0, 0);
	FRotator FireRotation = FireDirection.Rotation();
	FVector SpawnLocation = GetOwner()->GetActorLocation() + FireRotation.RotateVector(Player->GetGunOffset());

	GetWorld()->SpawnActor<ASpaceInvadersProjectile>(Projectile_BP, SpawnLocation, FireRotation);

	bCanFire = false;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &UShipWeaponComponent::ShotTimerExpired, FireRate);

	if (FireSound && GameMode->GetbIsSoundEffectsAllowed())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, Player->GetActorLocation());
	}
}

void UShipWeaponComponent::ShotTimerExpired()
{
	bCanFire = true;
}


