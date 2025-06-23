// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/PPGameSingleton.h"
#include "Skill/PPSkillDataManager.h"

UPPGameSingleton::UPPGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Purple/ProjectileData/ProjectileDataTable.ProjectileDataTable'"));
	if (DataTableRef.Object)
	{
		
		const UDataTable* DataTable = DataTableRef.Object;
		for (uint8 Index = 0; Index < static_cast<uint8>(EPlayerSkillType::Max); ++Index)
		{
			FString NameStr = StaticEnum<EPlayerSkillType>()->GetNameStringByValue(Index);
			FName RowName(*NameStr);

			EPlayerSkillType SkillEnum = static_cast<EPlayerSkillType>(Index);
			FPPProjectileData* FoundRow = DataTable->FindRow<FPPProjectileData>(RowName, TEXT("Skill Load"));
			if (FoundRow)
			{
				SkillDatas.FindOrAdd(SkillEnum, *FoundRow);

			}
			
			
			UE_LOG(LogTemp, Log, TEXT("GameSingleton : Skill Data -> %s"), *NameStr);

		}
		UE_LOG(LogTemp, Log, TEXT("=== [UPPGameSingleton] SkillDatas 등록 결과 ==="));

		for (const TPair<EPlayerSkillType, FPPProjectileData>& Pair : SkillDatas)
		{
			FString EnumName = StaticEnum<EPlayerSkillType>()->GetNameStringByValue(static_cast<uint8>(Pair.Key));
			const FPPProjectileData& Data = Pair.Value;

			// 원하는 필드값 추가해서 출력
			UE_LOG(LogTemp, Log, TEXT("[%-10s] Damage: %.1f, Speed: %.1f"), *EnumName, Data.Damage, Data.Speed);
		}

		
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillDataRef(TEXT("/Script/Engine.DataTable'/Game/Purple/SkillData/SkillDataTable.SkillDataTable'"));
	if (SkillDataRef.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillDataRef 로드 성공"));
		SkillData = SkillDataRef.Object;
		if (SkillData)
		{
			UE_LOG(LogTemp, Warning, TEXT("SkillData 유효함"));
			static const FString Context = TEXT("Skill Data Load");
			TArray<FPPSkillData*> AllRows;
			SkillData->GetAllRows<FPPSkillData>(Context, AllRows);

			for (const FPPSkillData* Row : AllRows)
			{
				if (!Row) continue;

				EPlayerSkillType SkillType = Row->SkillType;
				int32 Level = Row->Level;

				SkillDataMap.FindOrAdd(SkillType).Add(Level, *Row);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("===== [SkillDataMap 전체 데이터 출력] ====="));

	if (SkillDataMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillDataMap이 비어 있습니다."));
	}
	else
	{
		for (const TPair<EPlayerSkillType, TMap<int32, FPPSkillData>>& SkillPair : SkillDataMap)
		{
			EPlayerSkillType SkillType = SkillPair.Key;
			FString SkillTypeName = StaticEnum<EPlayerSkillType>()->GetNameStringByValue(static_cast<int32>(SkillType));
			const TMap<int32, FPPSkillData>& LevelMap = SkillPair.Value;

			if (LevelMap.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("SkillType [%s] 의 레벨 데이터가 비어 있습니다."), *SkillTypeName);
				continue;
			}

			for (const TPair<int32, FPPSkillData>& LevelPair : LevelMap)
			{
				int32 Level = LevelPair.Key;
				const FPPSkillData& Data = LevelPair.Value;

				UE_LOG(LogTemp, Log,
					TEXT("[SkillType: %-15s] Level: %2d | Cooldown: %5.2f | Damage: %6.2f | Range: %6.2f"),
					*SkillTypeName,
					Level,
					Data.Cooldown,
					Data.Damage,
					Data.Range
				);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("==========================================="));


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

const FPPSkillData* UPPGameSingleton::GetSkillData(EPlayerSkillType SkillType, int32 Level) const
{
	if (const TMap<int32, FPPSkillData>* LevelMap = SkillDataMap.Find(SkillType))
	{
		UE_LOG(LogTemp, Log, TEXT("In UseSkill"))

		return LevelMap->Find(Level);
	}
	return nullptr;
}
