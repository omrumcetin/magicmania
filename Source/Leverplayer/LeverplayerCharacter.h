// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeverplayerCharacter.generated.h"

class UInteractionComponent;

USTRUCT()
struct FInteractionData
{
    GENERATED_BODY()

    FInteractionData()
    {
		LastSeenInteractionComponent = nullptr;
        LastInteractionCheckTime = 0.f;
    }

    // Interaction component that we are viewing
    UPROPERTY()
    UInteractionComponent* LastSeenInteractionComponent;

    // The time we last checked for interactable
    UPROPERTY()
    float LastInteractionCheckTime;
};

UCLASS(config=Game)
class ALeverplayerCharacter : public ACharacter
{
	GENERATED_BODY()


#pragma region Interaction Checks

public:
    UPROPERTY()
    FInteractionData InteractionData;

    UPROPERTY(EditDefaultsOnly, Category = "Interaction", meta = (ClampMin = 0.f, ClampMax = 500.f))
    float InteractionCheckDistance;

    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    float InteractionCheckFrequency;

	FORCEINLINE UInteractionComponent* GetCurrentInteractable() const { return InteractionData.LastSeenInteractionComponent; };

    void PerformInteractionCheck();

    void CouldntFindInteractable();

    void FoundNewInteractable(UInteractionComponent* Interactable);

    void Interact();

#pragma endregion

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:

	virtual void Tick(float DeltaTime) override;

public:
	ALeverplayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

