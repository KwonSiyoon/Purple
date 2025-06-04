// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPProjectileSkill.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Projectile/PPProjectileBase.h"
#include "Character/PPCharacterBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



UPPProjectileSkill::UPPProjectileSkill()
{
    
}

void UPPProjectileSkill::UseSkill()
{

    if (!OwnerCharacter || !ProjectileClass) return;

    FVector CenterLocation = OwnerCharacter->GetActorLocation();

    // 스킬 레벨에 따른 발사체 개수 계산
    int32 ProjectileCount = BaseProjectileCount + (SkillLevel - 1) * ProjectilesPerLevel;
    float AngleStep = 360.0f / FMath::Max(1, ProjectileCount);

    FActorSpawnParameters Params;
    Params.Owner = OwnerCharacter;
    Params.Instigator = OwnerCharacter->GetInstigator();

    for (int32 i = 0; i < ProjectileCount; ++i)
    {
        float AngleDeg = -90.0f + AngleStep * i; // 시작 방향: -Y 축 (언리얼 기준으로 -90도)
        float Rad = FMath::DegreesToRadians(AngleDeg);

        FVector Direction = FVector(FMath::Cos(Rad), FMath::Sin(Rad), 0.0f);
        FVector SpawnLocation = CenterLocation + Direction * MuzzleOffset.X;
        FRotator SpawnRotation = Direction.Rotation();

        OwnerCharacter->GetWorld()->SpawnActor<APPProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
    }

    //const FPPProjectileData* ProjectileInfo = ProjectileDataTable->FindRow<FPPProjectileData>(
    //    TEXT("Fire"),
    //    TEXT("UseSkill_Projectile")
    //);
    //if (!ProjectileInfo || !ProjectileInfo->ProjectileClass) return;

    //// 원형으로 다방향 발사
    //const FVector Origin = GetActorLocation();
    //const FVector StartDir = -GetActorRightVector();
    //const float Radius = 100.f;
    //const int32 Count = GetLevelBasedProjectileCount();
    //const float AngleStep = 360.f / Count;

    //for (int32 i = 0; i < Count; ++i)
    //{
    //    const float AngleDeg = i * AngleStep;
    //    const FRotator Rot(0.f, AngleDeg, 0.f);
    //    const FVector Dir = Rot.RotateVector(StartDir).GetSafeNormal();

    //    const FVector SpawnLoc = Origin + Dir * Radius;
    //    const FRotator SpawnRot = Dir.Rotation();

    //    FActorSpawnParameters Params;
    //    Params.Owner = this;
    //    Params.Instigator = GetInstigator();

    //    APPProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<APPProjectileBase>(
    //        ProjectileInfo->ProjectileClass,
    //        FTransform(SpawnRot, SpawnLoc),
    //        this,
    //        GetInstigator()
    //    );

    //    if (Projectile)
    //    {
    //        Projectile->InitProjectile(*ProjectileInfo);
    //        UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SpawnRot, SpawnLoc));
    //    }
    //}
}

int32 UPPProjectileSkill::GetLevelBasedProjectileCount() const
{
    return 1 + SkillLevel;
}

