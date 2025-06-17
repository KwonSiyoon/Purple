// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "PPSkillData.h"
#include "PPPlayerSkillType.h"
#include "PPSkillDataManager.generated.h"

/**
 * 
 */
UCLASS()
class PURPLE_API UPPSkillDataManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UDataTable* InSkillDataTable);

	FPPSkillData GetSkillData(EPlayerSkillType SkillType, int32 Level) const;

private:
	TMap<EPlayerSkillType, TMap<int32, FPPSkillData>> SkillDataMap;
	
};
