// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/PPSkillBase.h"
#include "Projectile/PPProjectileData.h"
#include "PPPlayerSkillType.h"
#include "PPProjectileSkill.generated.h"


class APPProjectileBase;

/**
 * 
 */
UCLASS()
class PURPLE_API UPPProjectileSkill : public UPPSkillBase
{
	GENERATED_BODY()

public:
	UPPProjectileSkill();

    void SetProjectileClass(TSubclassOf<APPProjectileBase> InClass, EPlayerSkillType InType);
    // 외부에서 스킬 레벨을 설정할 수 있도록 공개
    void SetSkillLevel(int32 InLevel) { SkillLevel = InLevel; }

	virtual void UseSkill() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<APPProjectileBase> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    FVector MuzzleOffset = FVector(100.0f, 0.0f, 50.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TObjectPtr<UDataTable> ProjectileDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    EPlayerSkillType Type = EPlayerSkillType::None;

    // 기본 발사체 개수
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    int32 BaseProjectileCount = 3;

    // 스킬 레벨에 따른 추가 발사체 수
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    int32 ProjectilesPerLevel = 2;

    UPROPERTY(EditAnywhere, Category = "Skill")
    int32 SkillLevel = 1;

    int32 GetLevelBasedProjectileCount() const;

    float Cooldown;

};
