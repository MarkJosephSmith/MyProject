// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectCharacter.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AMyProjectCharacter::StaticClass();	
}
