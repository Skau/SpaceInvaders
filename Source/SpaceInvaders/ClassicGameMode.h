// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvadersGameMode.h"
#include "ClassicGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AClassicGameMode : public ASpaceInvadersGameMode
{
	GENERATED_BODY()

	AClassicGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
