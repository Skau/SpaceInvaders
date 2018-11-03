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
	
	//FHighScoreInfo highscore;
	//highscore.Name = "Spiller2";
	//highscore.BossKills = 17;
	//highscore.WaveReached = 25;
	//highscore.EnemiesKilled = 240;

	//UE_LOG(LogTemp, Warning, TEXT("TestFunction(highscore);"))
	//TestFunction(highscore);
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
void AHttpService::GetJsonStringFromStruct(StructType FilledStruct, FString & StringOutPut)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutPut, 0, 0, 0, 0);
}

template<typename StructType>
void AHttpService::GetStructFromJsonString(FHttpResponsePtr Response, StructType & StructOutPut)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutPut, 0, 0);
}

void AHttpService::TestFunction(FHighScoreInfo HighScoreInfo)
{
	FString ContentJsonString;
	GetJsonStringFromStruct(HighScoreInfo, ContentJsonString);
	TSharedRef<IHttpRequest> Request = PostRequest("highscore/add", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::TestFunctionRespone);
	Send(Request);

	FString FolderName = "Highscore";
	FString FileName = "PHPTestFile.txt";
	auto Directory = FPaths::ProjectDir() + "/" + FolderName;
	FString AbsolutePath = Directory + "/" + FileName;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Save to file
	if (FFileHelper::SaveStringToFile(ContentJsonString, *AbsolutePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sucessfully wrote to file"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed writing to file"))
	}
}

void AHttpService::TestFunctionRespone(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FHighScoreInfo HighScoreInfo;
	GetStructFromJsonString<FHighScoreInfo>(Response, HighScoreInfo);

	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *HighScoreInfo.Name);
	UE_LOG(LogTemp, Warning, TEXT("Bosses killed: %d"), HighScoreInfo.BossKills);
	UE_LOG(LogTemp, Warning, TEXT("Wave reached: %d"), HighScoreInfo.WaveReached);
	UE_LOG(LogTemp, Warning, TEXT("Enemies killed: %d"), HighScoreInfo.EnemiesKilled);
}
