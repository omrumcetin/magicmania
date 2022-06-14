// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/GeneralGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

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
            SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UGeneralGameInstance::OnCreateSessionComplete);
        }
    }
}

void UGeneralGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
    UE_LOG(LogTemp, Log, TEXT("Session Completed : %d"), Succeeded);
}

void UGeneralGameInstance::CreateServer()
{
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bIsLANMatch = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = 2;

    SessionPtr->CreateSession(0, FName("My Session"), SessionSettings);
}

void UGeneralGameInstance::JoinServer()
{

}
