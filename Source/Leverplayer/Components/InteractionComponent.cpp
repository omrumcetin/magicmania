// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "Players/LeverplayerCharacter.h"
#include "Leverplayer.h"
#include "InteractionComponent.h"
#include "UserInterface/InteractionWidget.h"

UInteractionComponent::UInteractionComponent()
{
    //No need to tick
    SetComponentTickEnabled(false);

    InteractionDistance = 400.f;

    AActor* owner = GetOwner();
    if (owner)
    {
        InteractiveNameText = FText::FromString(GetOwner()->GetName());
        UStaticMeshComponent* meshComponent = owner->FindComponentByClass<UStaticMeshComponent>();
        if (meshComponent)
        {
            meshComponent->SetCollisionResponseToChannel(COLLISION_INTERACTION, ECR_Block);
        }
    }
    else
    {
        InteractiveNameText = FText::FromString("Intractable Object");
    }

    InteractiveActionText = FText::FromString("Interact");

    Space = EWidgetSpace::Screen;
    DrawSize = FIntPoint(1000, 300);
    bDrawAtDesiredSize = true;

    SetActive(true);
    SetHiddenInGame(true);
}

bool UInteractionComponent::CanInteract(ALeverplayerCharacter* Character)
{
    return IsActive() && GetOwner() != nullptr && Character != nullptr;
}

void UInteractionComponent::Interact(ALeverplayerCharacter* Character)
{
    if (!CanInteract(Character))
    {
        return;
    }
    OnInteract.Broadcast(Character);
}

void UInteractionComponent::RefreshWidget()
{
    UInteractionWidget* interactionWidget = Cast<UInteractionWidget>(GetUserWidgetObject());

    if (interactionWidget)
    {
        if (!bHiddenInGame)
        {
            interactionWidget->SetVisibility(ESlateVisibility::Visible);
            interactionWidget->UpdateInteractionWidget(this);
        }
        else
        {
            interactionWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UInteractionComponent::BeginFocus(ALeverplayerCharacter* Character)
{
    if (!CanInteract(Character))
    {
        return;
    }
    OnBeginFocus.Broadcast(Character);
    SetHiddenInGame(false);
    RefreshWidget();
}

void UInteractionComponent::EndFocus(ALeverplayerCharacter* Character)
{
    SetHiddenInGame(true);
    OnEndFocus.Broadcast(Character);
    RefreshWidget();
}