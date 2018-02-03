// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvadersGameInstance.h"

int USpaceInvadersGameInstance::GetTotalWaves()
{
	return TotalWaves;
}

void USpaceInvadersGameInstance::SetTotalWaves(int NewTotalWaves)
{
	TotalWaves = NewTotalWaves;
}

int USpaceInvadersGameInstance::GetSpawnRate()
{
	return SpawnRate;
}

void USpaceInvadersGameInstance::SetSpawnRate(int NewSpawnRate)
{
	SpawnRate = NewSpawnRate;
}

bool USpaceInvadersGameInstance::GetMusicAllowed()
{
	return bIsMusicAllowed;
}

void USpaceInvadersGameInstance::SetMusicAllowed(bool value)
{
	bIsMusicAllowed = value;
}

