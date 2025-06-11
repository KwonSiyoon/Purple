#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PPSkillData.generated.h"

USTRUCT(BlueprintType)
struct FPPSkillData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsAuto;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ProjectileAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<class APPProjectileBase> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraSystem> SkillEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> Icon;


};