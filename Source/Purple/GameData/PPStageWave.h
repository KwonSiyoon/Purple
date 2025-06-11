#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PPStageWave.generated.h"

USTRUCT(BlueprintType)
struct FPPStageWave : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Time;  // 웨이브 발생 시간

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<class APPEnemyCharacterBase> EnemyBlueprint;  // 어떤 적을 소환할지

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Interval;  // 소환 간격

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName PatternID;  // SpawnPattern 테이블 RowName 참조

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsElite;
};
