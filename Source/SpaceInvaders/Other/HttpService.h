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

UCLASS()
class SPACEINVADERS_API AHttpService : public AActor
{
	GENERATED_BODY()
	
public:	
	AHttpService();

	virtual void BeginPlay() override;

	void AddDataToHighscore(FHighScoreInfo HighScoreInfo);
	void AddDataToHighScoreResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetHighScores();
	void GetHighScoresResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSucessful);

private:
	FHttpModule* Http;
	FString ApiBaseUrl = "http://localhost/stuff/spaceinvaders/";
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
	void GetJsonStringFromStruct(FHighScoreInfo FilledStruct, FString& StringOutPut);
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutPut);
	void GetStructFromJsonString(FHttpResponsePtr Response, FHighScoreInfo& StructOutPut);
	void GetArrayOfStructFromJsonString(FHttpResponsePtr Response, TArray<FHighScoreInfo>& OutArray);
};


