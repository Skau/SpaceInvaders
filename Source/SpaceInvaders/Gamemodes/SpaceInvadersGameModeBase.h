// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceInvadersGameModeBase.generated.h"

class AHttpService;

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ASpaceInvadersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	AHttpService* GetHttpService() { return HttpService; }

protected:
	virtual void BeginPlay() override;

	AHttpService* HttpService;
	
};
