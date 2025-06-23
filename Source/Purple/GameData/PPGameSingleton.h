// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Projectile/PPProjectileData.h"
#include "Skill/PPPlayerSkillType.h"
#include "Skill/PPSkillData.h"
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

	UPROPERTY()
	TObjectPtr<class UPPSkillDataManager> SkillDataManager;

	UPROPERTY()
	TObjectPtr<class UDataTable> SkillData;

	const FPPSkillData* GetSkillData(EPlayerSkillType SkillType, int32 Level) const;

	TMap<EPlayerSkillType, TMap<int32, FPPSkillData>> SkillDataMap;


};
