// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlackHolePower0GameMode.h"
#include "BlackHolePower0Character.h"
#include "UObject/ConstructorHelpers.h"

ABlackHolePower0GameMode::ABlackHolePower0GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
