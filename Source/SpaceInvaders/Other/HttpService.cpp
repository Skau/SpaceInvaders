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

void AHttpService::GetJsonStringFromHighScoreInfo(FHighScoreInfo FilledStruct, FString & StringOutPut)
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

void AHttpService::GetHighScoreInfoFromJsonString(FHttpResponsePtr Response, FHighScoreInfo & StructOutPut)
{
}

void AHttpService::GetArrayOfHighScoreInfoFromJsonString(FHttpResponsePtr Response, TArray<FHighScoreInfo>& OutArray)
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
				data.EnemiesKilled = temp->GetNumberField("EnemyKills");
				OutArray.Add(data);
			}
		}
	}
}

void AHttpService::RequestAddHighscoreToDatabase(FHighScoreInfo HighScoreInfo)
{
	FString ContentJsonString;
	GetJsonStringFromHighScoreInfo(HighScoreInfo, ContentJsonString);
	TSharedRef<IHttpRequest> Request = PostRequest("addData.php/", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::ResponseAddHighscoreToDatabase);
	Send(Request);
}

void AHttpService::ResponseAddHighscoreToDatabase(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Successfully added score to database"))
}

void AHttpService::RequestHighscoresFromDatabase()
{
	TSharedRef<IHttpRequest> Request = GetRequest("getData.php?data=getALL");
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::ResponseHighscoresFromDatabase);
	Send(Request);
}

void AHttpService::ResponseHighscoresFromDatabase(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSucessful)
{
	if (!ResponseIsValid(Response, bWasSucessful)) return;


	auto GameMode = Cast<AMainMenuGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		TArray<FHighScoreInfo> HighScoreInfo;
	
		GetArrayOfHighScoreInfoFromJsonString(Response, HighScoreInfo);

		UE_LOG(LogTemp, Warning, TEXT("Successfully retrieved highscores from databse"))

		// Converting to struct that main menu expects
		TArray<FHighScoreData> Highscores;
		for (auto HighScore : HighScoreInfo)
		{
			FHighScoreData data;
			data.PlayerName = HighScore.Name;
			data.BossesKilled = HighScore.BossKills;
			data.EnemiesKilled = HighScore.EnemiesKilled;
			data.WaveReached = HighScore.WaveReached;
			Highscores.Add(data);
		}
		GameMode->LoadHighScore(Highscores);
	}
}
