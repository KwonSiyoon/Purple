#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PPSpawnPattern.generated.h"

UENUM(BlueprintType)
enum class ESpawnPatternType : uint8
{
    Circle,
    FrontArc,
    RearAttack,
    FixedZone,
    OffscreenRandom
};

USTRUCT(BlueprintType)
struct FPPSpawnPattern : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ESpawnPatternType PatternType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Count;  // 이 패턴으로 몇 마리를 소환할지

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FVector> FixedPoints;  // FixedZone용
};
