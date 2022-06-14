// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Lever.h"
#include "Components/InteractionComponent.h"
#include "World/Door.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));
	SetRootComponent(Lever);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetupAttachment(Lever);
	InteractionComponent->InteractionDistance = 400.f;
	InteractionComponent->InteractiveNameText = FText::FromString("Lever");
	InteractionComponent->InteractiveActionText = FText::FromString("Use");
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent->OnInteract.AddDynamic(this, &ALever::Interact);
}

void ALever::Interact(ALeverplayerCharacter* Character)
{
	if (!Door)
	{
		UE_LOG(LogTemp, Warning, TEXT("# %s # Cannot find the reference of door"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	Door->ToggleDoor();
}
