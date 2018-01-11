// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersGameMode.h"
#include "SpaceInvadersPawn.h"

ASpaceInvadersGameMode::ASpaceInvadersGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ASpaceInvadersPawn::StaticClass();
}

