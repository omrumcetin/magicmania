// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/GeneralGameInstance.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"

UGeneralGameInstance::UGeneralGameInstance()
{

}

void UGeneralGameInstance::Init()
{
    if (IOnlineSubsystem* subSystem = IOnlineSubsystem::Get())
    {
        SessionPtr = subSystem->GetSessionInterface();
        if (SessionPtr.IsValid())
        {
            SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);
            SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionsComplete);
            SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionComplete);
        }
    }
}

void UGeneralGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
    if (Succeeded)
    {
        GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
    }
}

void UGeneralGameInstance::OnFindSessionsComplete(bool Succeeded)
{
    if (Succeeded)
    {
        TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
        UE_LOG(LogTemp, Warning, TEXT("Found servers, count : %d"), SearchResults.Num());
        if (SearchResults.Num())
        {
            FOnlineSessionSearchResult result = SearchResults[0];
            SessionPtr->JoinSession(0, FName("TestSession"), result);
        }
    }
}

void UGeneralGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        FString joinAddress = "";
        SessionPtr->GetResolvedConnectString(SessionName, joinAddress);
        if (joinAddress != "")
        {
            PC->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
        }
    }
}

void UGeneralGameInstance::CreateServer()
{
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bIsLANMatch = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = 5;

    SessionPtr->CreateSession(0, FName("TestSession"), SessionSettings);
}

void UGeneralGameInstance::FindSessions()
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = true;
    SessionSearch->MaxSearchResults = 1000;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    SessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
}