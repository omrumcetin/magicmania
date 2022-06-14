// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GeneralGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEVERPLAYER_API UGeneralGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGeneralGameInstance();

protected:
    virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);

protected:

	IOnlineSessionPtr SessionPtr;


	UFUNCTION(BlueprintCallable, Category = "Server")
	void CreateServer();

	UFUNCTION(BlueprintCallable, Category = "Server")
	void JoinServer();
};
