// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionComponent.generated.h"

class ALeverplayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, ALeverplayerCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, ALeverplayerCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, ALeverplayerCharacter*, Character);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEVERPLAYER_API UInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UInteractionComponent();

protected:

	bool CanInteract(ALeverplayerCharacter* Character);

	UPROPERTY()
	ALeverplayerCharacter* Interactor;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Component", meta = (ClampMin = 300.f))
    float InteractionDistance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Component")
    FText InteractiveNameText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Component")
    FText InteractiveActionText;

    UPROPERTY(BlueprintAssignable)
    FOnBeginFocus OnBeginFocus;

    UPROPERTY(BlueprintAssignable)
    FOnEndFocus OnEndFocus;

    UPROPERTY(BlueprintAssignable)
    FOnInteract OnInteract;

    UFUNCTION(BlueprintCallable, Category = "Interaction Component")
    ALeverplayerCharacter* GetInteractor() const { return Interactor; };

    void BeginFocus(ALeverplayerCharacter* Character);

    void EndFocus(ALeverplayerCharacter* Character);

    void Interact(ALeverplayerCharacter* Character);

    void RefreshWidget();
};
