// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"

#include "Gamemodes/SpaceInvadersGameMode.h"
#include "PlayersAndEnemies/SpaceInvadersPawn.h"
#include "OtherGameActors/SpaceInvadersProjectile.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/ProjectileSound.ProjectileSound"));
	FireSound = FireAudio.Object;
	FireRate = .25f;
	bCanFire = true;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<ASpaceInvadersGameMode>(GetWorld()->GetAuthGameMode());
	Player = Cast<ASpaceInvadersPawn>(GetOwner());
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::Fire()
{
	if (!Player || !GameMode) { return; }

	if (bCanFire && Projectile_BP)
	{
		FireShot();
	}
}

void UWeaponComponent::FireShot()
{
	FVector FireDirection = FVector(1, 0, 0);
	FRotator FireRotation = FireDirection.Rotation();
	FVector SpawnLocation = GetOwner()->GetActorLocation() + FireRotation.RotateVector(Player->GetGunOffset());

	GetWorld()->SpawnActor<ASpaceInvadersProjectile>(Projectile_BP, SpawnLocation, FireRotation);

	bCanFire = false;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &UWeaponComponent::ShotTimerExpired, FireRate);

	if (FireSound && GameMode->GetbIsSoundEffectsAllowed())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, Player->GetActorLocation());
	}
}

void UWeaponComponent::ShotTimerExpired()
{
	bCanFire = true;
}


