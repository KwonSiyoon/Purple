#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PPProjectileData.generated.h"

USTRUCT(BlueprintType)
struct FPPProjectileData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class APPProjectileBase> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LifeSpan;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UNiagaraSystem> TrailEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UNiagaraSystem> ImpactEffect;
};