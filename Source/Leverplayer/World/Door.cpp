// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Door.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	SetRootComponent(DoorFrame);

	DoorSelf = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorSelf->SetupAttachment(DoorFrame);

	bRequestedOpen = false;

    bReplicates = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bRequestedOpen);
}

void ADoor::SwapDoor()
{
	if (bRequestedOpen)
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}

}

void ADoor::ToggleDoor()
{
	bRequestedOpen = !bRequestedOpen ;
	SwapDoor();
}

