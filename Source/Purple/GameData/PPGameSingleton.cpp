// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/PPGameSingleton.h"

UPPGameSingleton::UPPGameSingleton()
{
}

UPPGameSingleton& UPPGameSingleton::Get()
{
	UPPGameSingleton* Singleton = Cast<UPPGameSingleton>(GEngine->GameSingleton);

	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Error, TEXT("Invalid Game Singleton."));

	return *NewObject<UPPGameSingleton>();
}
