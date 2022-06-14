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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Replicated, VisibleAnywhere, ReplicatedUsing = "SwapDoor", Category = "Door")
	bool bRequestedOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category	= "Door")
	UStaticMeshComponent* DoorFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorSelf;

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerToggleDoor();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    UFUNCTION(BlueprintImplementableEvent)
    void OpenDoor();

    UFUNCTION(BlueprintImplementableEvent)
    void CloseDoor();

	UFUNCTION()
	void SwapDoor();

public:	

	UFUNCTION()
	void ToggleDoor();
};
