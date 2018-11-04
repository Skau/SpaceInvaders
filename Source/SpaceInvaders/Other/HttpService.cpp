// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpService.h"
#include "Engine/World.h"
#include "Gamemodes/MainMenuGameMode.h"

AHttpService::AHttpService()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHttpService::BeginPlay()
{
	Super::BeginPlay();
	Http = &FHttpModule::Get();
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
	//Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader("Content-Type", "application/json");
	//Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> AHttpService::GetRequest(FString Subroute)
{
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest> AHttpService::PostRequest(FString Subroute, FString ContentJsonString)
{
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
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
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutPut, 0, 0, 0, 0);
}

void AHttpService::GetJsonStringFromStruct(FHighScoreInfo FilledStruct, FString & StringOutPut)
{
	TSharedPtr<FJsonObject> NewPlayerDataJsonObject = MakeShareable(new FJsonObject);
	NewPlayerDataJsonObject->SetStringField("Name", FilledStruct.Name);
	NewPlayerDataJsonObject->SetNumberField("BossKills", FilledStruct.BossKills);
	NewPlayerDataJsonObject->SetNumberField("WaveReached", FilledStruct.WaveReached);
	NewPlayerDataJsonObject->SetNumberField("EnemiesKilled", FilledStruct.EnemiesKilled);

	TSharedRef <TJsonWriter<TCHAR>> JsonWriter =
		TJsonWriterFactory<>::Create(&StringOutPut);

	FJsonSerializer::Serialize(NewPlayerDataJsonObject.ToSharedRef(), JsonWriter);
}

template<typename StructType>
void AHttpService::GetStructFromJsonString(FHttpResponsePtr Response, StructType & StructOutPut)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutPut, 0, 0);
}

void AHttpService::GetStructFromJsonString(FHttpResponsePtr Response, FHighScoreInfo & StructOutPut)
{

}
void AHttpService::GetArrayOfStructFromJsonString(FHttpResponsePtr Response, TArray<FHighScoreInfo>& OutArray)
{
	FString JsonString = Response->GetContentAsString();
	if (JsonString.Len())
	{
		TSharedPtr<FJsonObject> JSonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JSonObject))
		{
			auto PlayerData = JSonObject->GetArrayField("players");
			for (auto& player : PlayerData)
			{
				auto temp = player->AsObject();
				FHighScoreInfo data;
				data.Name = temp->GetStringField("Name");
				data.BossKills = temp->GetNumberField("BossKills");
				data.WaveReached = temp->GetNumberField("WaveReached");
				data.EnemiesKilled = temp->GetNumberField("EnemiesKilled");
				OutArray.Add(data);
				// TODO Fix so main menu game mode gets all these
			}
		}
	}
}

void AHttpService::AddDataToHighscore(FHighScoreInfo HighScoreInfo)
{
	FString ContentJsonString;
	GetJsonStringFromStruct(HighScoreInfo, ContentJsonString);
	TSharedRef<IHttpRequest> Request = PostRequest("addData.php/", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::AddDataToHighScoreResponse);
	Send(Request);
}

void AHttpService::AddDataToHighScoreResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Successfully added score to database"))
}

void AHttpService::GetHighScores()
{
	TSharedRef<IHttpRequest> Request = GetRequest("getData.php?data=getALL");
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::GetHighScoresResponse);
	Send(Request);
}

void AHttpService::GetHighScoresResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSucessful)
{
	if (!ResponseIsValid(Response, bWasSucessful)) return;
	auto GameMode = Cast<AMainMenuGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString())

		TArray<FHighScoreInfo> HighScoreInfo;
	
		// TODO Get array from db to high score info array and give to main menu game mode
		GetArrayOfStructFromJsonString(Response, HighScoreInfo);

		TArray<FHighScoreDataMM> Highscores;
		for (auto HighScore : HighScoreInfo)
		{
			FHighScoreDataMM data;
			data.PlayerName = HighScore.Name;
			data.BossesKilled = HighScore.BossKills;
			data.EnemiesKilled = HighScore.EnemiesKilled;
			data.WaveReached = HighScore.WaveReached;
			Highscores.Add(data);
		}
		GameMode->LoadHighScore(Highscores);
	}
}
