// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GeneralGameInstance.generated.h"

class FOnlineSessionSearchResult;

UCLASS()
class LEVERPLAYER_API UGeneralGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGeneralGameInstance();

protected:
    virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	
	virtual void OnFindSessionsComplete(bool Succeeded);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:

    IOnlineSessionPtr SessionPtr;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

public:

	UFUNCTION(BlueprintCallable, Category = "Instance")
	void CreateServer();

	UFUNCTION(BlueprintCallable, Category = "Instance")
	void FindSessions();

};
