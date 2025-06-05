// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/PPGameSingleton.h"

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
