// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPProjectileSkill.h"
#include "Projectile/PPProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"


APPProjectileSkill::APPProjectileSkill()
{
    
}

void APPProjectileSkill::UseSkill()
{
    /*const FName SkillRowName = GetSkillRowNameByType(SkillType);
    const FSkillData* SkillInfo = SkillDataTable->FindRow<FSkillData>(SkillRowName, TEXT("UseSkill"));
    if (!SkillInfo) return;*/

    const FPPProjectileData* ProjectileInfo = ProjectileDataTable->FindRow<FPPProjectileData>(
        TEXT("Fire"),
        TEXT("UseSkill_Projectile")
    );
    if (!ProjectileInfo || !ProjectileInfo->ProjectileClass) return;

    // 원형으로 다방향 발사
    const FVector Origin = GetActorLocation();
    const FVector StartDir = -GetActorRightVector();
    const float Radius = 100.f;
    const int32 Count = GetLevelBasedProjectileCount();
    const float AngleStep = 360.f / Count;

    for (int32 i = 0; i < Count; ++i)
    {
        const float AngleDeg = i * AngleStep;
        const FRotator Rot(0.f, AngleDeg, 0.f);
        const FVector Dir = Rot.RotateVector(StartDir).GetSafeNormal();

        const FVector SpawnLoc = Origin + Dir * Radius;
        const FRotator SpawnRot = Dir.Rotation();

        FActorSpawnParameters Params;
        Params.Owner = this;
        Params.Instigator = GetInstigator();

        APPProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<APPProjectileBase>(
            ProjectileInfo->ProjectileClass,
            FTransform(SpawnRot, SpawnLoc),
            this,
            GetInstigator()
        );

        if (Projectile)
        {
            Projectile->InitProjectile(*ProjectileInfo);
            UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SpawnRot, SpawnLoc));
        }
    }
}

int32 APPProjectileSkill::GetLevelBasedProjectileCount() const
{
    return 1 + SkillLevel;
}

