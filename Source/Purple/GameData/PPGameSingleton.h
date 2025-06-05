// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Projectile/PPProjectileData.h"
#include "Skill/PPPlayerSkillType.h"
#include "PPGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class PURPLE_API UPPGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UPPGameSingleton();

	static UPPGameSingleton& Get();

	TMap<EPlayerSkillType, FPPProjectileData> SkillDatas;


};
