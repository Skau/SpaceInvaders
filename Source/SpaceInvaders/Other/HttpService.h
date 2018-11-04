// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HighscoreSaver.h"
#include "HttpService.generated.h"

class AMainMenuGameMode;

UCLASS(Blueprintable, hideCategories = (Rendering, Replication, Input, Actor, "Actor Tick"))
class SPACEINVADERS_API AHttpService : public AActor
{
	GENERATED_BODY()
	
public:	
	AHttpService();

	virtual void BeginPlay() override;

	void RequestAddHighscoreToDatabase(FHighScoreInfo HighScoreInfo);

	UFUNCTION(BlueprintCallable)
	void RequestHighscoresFromDatabase();

private:
	void ResponseAddHighscoreToDatabase(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void ResponseHighscoresFromDatabase(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSucessful);

	FHttpModule* Http;
	FString ApiBaseUrl = "http://localhost/games/spaceinvaders/";
	FString AuthorizationHeader = TEXT("Autorization");
	void SetAutorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> RequestWithRoute(FString Subroute);
	void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	TSharedRef<IHttpRequest> PostRequest(FString Subroute, FString ContentJsonString);
	void Send(TSharedRef<IHttpRequest>& Request);

	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutPut);
	
	void GetJsonStringFromHighScoreInfo(FHighScoreInfo FilledStruct, FString& StringOutPut);
	
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutPut);
	
	void GetHighScoreInfoFromJsonString(FHttpResponsePtr Response, FHighScoreInfo& StructOutPut);

	void GetArrayOfHighScoreInfoFromJsonString(FHttpResponsePtr Response, TArray<FHighScoreInfo>& OutArray);
};


