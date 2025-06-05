// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPProjectileSkill.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Projectile/PPProjectileBase.h"
#include "Character/PPCharacterBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Skill/PPPlayerSkillType.h"


UPPProjectileSkill::UPPProjectileSkill()
{
    
}

void UPPProjectileSkill::UseSkill()
{
    UE_LOG(LogTemp, Log, TEXT("UseSkill In UPPProjectileSkill."));
    if (!OwnerCharacter || !ProjectileClass) return;
    UE_LOG(LogTemp, Log, TEXT("UseSkill In UPPProjectileSkill And Set ProjectileClass."));

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

}

int32 UPPProjectileSkill::GetLevelBasedProjectileCount() const
{
    return 1 + SkillLevel;
}

