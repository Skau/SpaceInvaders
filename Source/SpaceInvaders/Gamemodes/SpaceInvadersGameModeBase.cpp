// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvadersGameModeBase.h"
#include "Engine/World.h"

#include "Other/HttpService.h"

void ASpaceInvadersGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HttpService = GetWorld()->SpawnActor<AHttpService>();
}

