// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPSkillDataManager.h"

void UPPSkillDataManager::Initialize(UDataTable* InSkillDataTable)
{
    SkillDataMap.Empty();

    if (!InSkillDataTable) return;

    static const FString Context = TEXT("Skill Data Load");
    TArray<FPPSkillData*> AllRows;
    InSkillDataTable->GetAllRows<FPPSkillData>(Context, AllRows);

    for (const FPPSkillData* Row : AllRows)
    {
        if (!Row) continue;

        // EPlayerSkillType를 FName에서 추출
        FString SkillNameStr = Row->SkillID.ToString();
        EPlayerSkillType SkillType = StaticEnum<EPlayerSkillType>()->GetValueByNameString(SkillNameStr) != INDEX_NONE
            ? static_cast<EPlayerSkillType>(StaticEnum<EPlayerSkillType>()->GetValueByNameString(SkillNameStr))
            : EPlayerSkillType::Empty;

        SkillDataMap.FindOrAdd(SkillType).Add(Row->Level, *Row);
    }
}

FPPSkillData UPPSkillDataManager::GetSkillData(EPlayerSkillType SkillType, int32 Level) const
{
    if (const TMap<int32, FPPSkillData>* LevelMap = SkillDataMap.Find(SkillType))
    {
        return *LevelMap->Find(Level);
    }
    return FPPSkillData();
}