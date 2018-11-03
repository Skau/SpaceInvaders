// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpService.h"

AHttpService::AHttpService()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHttpService::BeginPlay()
{
	Super::BeginPlay();
	Http = &FHttpModule::Get();
	
	FHighScoreInfo highscore;
	highscore.PlayerName = "Spiller1";
	highscore.BossesKilled = 17;
	highscore.WaveReached = 25;
	highscore.EnemiesKilled = 240;

	UE_LOG(LogTemp, Warning, TEXT("TestFunction(highscore);"))
	TestFunction(highscore);
}

void AHttpService::TestFunction(FHighScoreInfo HighScoreInfo)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FHighScoreInfo>(HighScoreInfo, ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("data", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::TestFunctionRespone);
	Send(Request);
}

void AHttpService::TestFunctionRespone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FHighScoreInfo HighScoreInfo;
	GetStructFromJsonString<FHighScoreInfo>(Response, HighScoreInfo);

	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *HighScoreInfo.PlayerName);
	UE_LOG(LogTemp, Warning, TEXT("Bosses killed: %d"), HighScoreInfo.BossesKilled);
	UE_LOG(LogTemp, Warning, TEXT("Wave reached: %d"), HighScoreInfo.WaveReached);
	UE_LOG(LogTemp, Warning, TEXT("Enemies killed: %d"), HighScoreInfo.EnemiesKilled);
}

void AHttpService::SetAutorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request)
{
	Request->SetHeader(AuthorizationHeader, Hash);
}

TSharedRef<IHttpRequest> AHttpService::RequestWithRoute(FString Subroute)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	SetRequestHeaders(Request);
	return Request;
}

void AHttpService::SetRequestHeaders(TSharedRef<IHttpRequest>& Request)
{
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> AHttpService::GetRequest(FString Subroute)
{
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("Get");
	return Request;
}

TSharedRef<IHttpRequest> AHttpService::PostRequest(FString Subroute, FString ContentJsonString)
{
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	return Request;
}

void AHttpService::Send(TSharedRef<IHttpRequest>& Request)
{
	Request->ProcessRequest();
}

bool AHttpService::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}

}

template<typename StructType>
void AHttpService::GetJsonStringFromStruct(StructType FilledStruct, FString & StringOutPut)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutPut, 0, 0);
}

template<typename StructType>
void AHttpService::GetStructFromJsonString(FHttpResponsePtr Response, StructType & StructOutPut)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutPut, 0, 0);
}
