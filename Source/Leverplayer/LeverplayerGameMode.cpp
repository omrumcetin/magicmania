// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeverplayerGameMode.h"
#include "LeverplayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALeverplayerGameMode::ALeverplayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
