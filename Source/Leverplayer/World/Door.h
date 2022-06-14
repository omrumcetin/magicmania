// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class LEVERPLAYER_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

private:
	bool bDoorStatus;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void OpenDoor();

    UFUNCTION()
    void CloseDoor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ToggleDoor();
};
