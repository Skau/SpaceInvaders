// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ASpaceInvadersPawn;
class ASpaceInvadersGameMode;
class ASpaceInvadersProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEINVADERS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void Fire();

protected:	
	UWeaponComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void FireShot();

	void ShotTimerExpired();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpaceInvadersProjectile> Projectile_BP;

	UPROPERTY(EditDefaultsOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* FireSound;

	ASpaceInvadersPawn* Player;

	ASpaceInvadersGameMode* GameMode;

	bool bCanFire;
	FTimerHandle TimerHandle_ShotTimerExpired;
};
