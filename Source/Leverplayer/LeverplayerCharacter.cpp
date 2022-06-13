// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeverplayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Leverplayer.h"
#include "Components/InteractionComponent.h"

static bool InteractionDebug = false;
FAutoConsoleVariableRef CVARInteractionDebug(
    TEXT("Interaction.Debug"),
    InteractionDebug,
    TEXT("Draw debug line for interaction casts"),
    ECVF_Cheat);

ALeverplayerCharacter::ALeverplayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    //Optimization for checking frequency
    InteractionCheckFrequency = 0.5f;
    InteractionCheckDistance = 500.f;
}

void ALeverplayerCharacter::Tick(float DeltaTime)
{
    if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }
}

void ALeverplayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ALeverplayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ALeverplayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ALeverplayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ALeverplayerCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALeverplayerCharacter::Interact);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALeverplayerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALeverplayerCharacter::TouchStopped);
}

void ALeverplayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ALeverplayerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ALeverplayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALeverplayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALeverplayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALeverplayerCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ALeverplayerCharacter::PerformInteractionCheck()
{
    if (GetController() == nullptr)
    {
        return;
    }

    InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector eyesLocation;
    FRotator eyesRotation;

    GetController()->GetPlayerViewPoint(eyesLocation, eyesRotation);

    auto traceStart = eyesLocation;
    auto traceEnd = (eyesRotation.Vector() * InteractionCheckDistance) + traceStart;

    FHitResult traceHit;

    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(traceHit, eyesLocation, traceEnd, COLLISION_INTERACTION, queryParams))
    {
        // Check if we hit an actor
        AActor* hitActor = traceHit.GetActor();
        if (!hitActor)
        {
            return;
        }
		if (InteractionDebug)
		{
            DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, 1.f, ECC_WorldStatic, 1.f);
		}

        // Check if actor has interaction component
        UInteractionComponent* interactionComponent = Cast<UInteractionComponent>(hitActor->GetComponentByClass(UInteractionComponent::StaticClass()));
        if (!interactionComponent)
        {
            CouldntFindInteractable();
            return;
        }

        if (!interactionComponent->IsActive())
        {
            CouldntFindInteractable();
            return;
        }

        float distance = (traceStart - traceHit.ImpactPoint).Size();

        if (interactionComponent != GetCurrentInteractable() && distance <= interactionComponent->InteractionDistance)
        {
            FoundNewInteractable(interactionComponent);
        }

        else if (distance > interactionComponent->InteractionDistance && GetCurrentInteractable())
        {
            CouldntFindInteractable();
        }
        return;
    }

    CouldntFindInteractable();
}

void ALeverplayerCharacter::CouldntFindInteractable()
{
    UInteractionComponent* interactable = GetCurrentInteractable();
    if (interactable)
    {
        interactable->EndFocus(this);
    }

    InteractionData.LastSeenInteractionComponent = nullptr;
}

void ALeverplayerCharacter::FoundNewInteractable(UInteractionComponent* Interactable)
{
    UInteractionComponent* oldInteractable = GetCurrentInteractable();
    if (oldInteractable)
    {
        oldInteractable->EndFocus(this);
    }

    Interactable->BeginFocus(this);
    InteractionData.LastSeenInteractionComponent = Interactable;

    UE_LOG(LogTemp, Log, TEXT("Found interactable %s"), *Interactable->GetOwner()->GetName());
}

void ALeverplayerCharacter::Interact()
{
    UInteractionComponent* interactable = GetCurrentInteractable();
    if (!interactable)
    {
        return;
    }
    interactable->Interact(this);
}
