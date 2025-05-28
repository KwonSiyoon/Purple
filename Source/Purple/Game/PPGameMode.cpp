// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PPGameMode.h"

APPGameMode::APPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(TEXT("/Script/Purple.PPCharacterBase"));
	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Purple.PPPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

}
