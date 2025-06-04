// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/PPSkillBase.h"
#include "Projectile/PPProjectileData.h"
#include "PPProjectileSkill.generated.h"

/**
 * 
 */
UCLASS()
class PURPLE_API APPProjectileSkill : public APPSkillBase
{
	GENERATED_BODY()

public:
	APPProjectileSkill();

	virtual void UseSkill() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<class APPProjectileBase> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TObjectPtr<UDataTable> ProjectileDataTable;

    UPROPERTY(EditAnywhere, Category = "Skill")
    int32 SkillLevel = 1;

    int32 GetLevelBasedProjectileCount() const;


};
