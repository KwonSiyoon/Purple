// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPProjectileSkill.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Projectile/PPProjectileBase.h"
#include "Character/PPCharacterBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameData/PPGameSingleton.h"


UPPProjectileSkill::UPPProjectileSkill()
{
    
}

void UPPProjectileSkill::SetProjectileClass(TSubclassOf<APPProjectileBase> InClass, EPlayerSkillType InType)
{
    ProjectileClass = InClass;
    Type = InType;
}


void UPPProjectileSkill::UseSkill()
{
    if (!OwnerCharacter || !ProjectileClass) return;

    FVector CenterLocation = OwnerCharacter->GetActorLocation();
    int32 ProjectileCount = BaseProjectileCount + (SkillLevel - 1) * ProjectilesPerLevel;
    float AngleStep = 360.0f / FMath::Max(1, ProjectileCount);

    for (int32 i = 0; i < ProjectileCount; ++i)
    {
        float AngleDeg = AngleStep * i;
        float Rad = FMath::DegreesToRadians(AngleDeg);

        FVector BaseDirection = FVector(FMath::Sin(Rad), -FMath::Cos(Rad), 0.0f); // 월드 기준 -Y 방향 시작
        FVector SpawnLocation = CenterLocation + BaseDirection * 50.0f + FVector(0.0f, 0.0f, MuzzleOffset.Z);
        FRotator SpawnRotation = FRotationMatrix::MakeFromX(BaseDirection).Rotator(); // ✅ 정확한 회전

        FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

        APPProjectileBase* ProjectileObject = GetWorld()->SpawnActorDeferred<APPProjectileBase>(ProjectileClass, SpawnTransform, OwnerCharacter, OwnerCharacter->GetInstigator());
        if (ProjectileObject)
        {
            ProjectileObject->InitProjectile(UPPGameSingleton::Get().SkillDatas[Type]);

            ProjectileObject->FinishSpawning(SpawnTransform);

            ProjectileObject->SetActorRotation(SpawnRotation);
        }
    }

    //UE_LOG(LogTemp, Log, TEXT("UseSkill In UPPProjectileSkill."));
    //if (!OwnerCharacter || !ProjectileClass) return;
    //UE_LOG(LogTemp, Log, TEXT("UseSkill In UPPProjectileSkill And Set ProjectileClass."));

    //FVector CenterLocation = OwnerCharacter->GetActorLocation();

    //// 스킬 레벨에 따른 발사체 개수 계산
    //int32 ProjectileCount = BaseProjectileCount + (SkillLevel - 1) * ProjectilesPerLevel;
    //float AngleStep = 360.0f / FMath::Max(1, ProjectileCount);

    //FActorSpawnParameters Params;
    //Params.Owner = OwnerCharacter;
    //Params.Instigator = OwnerCharacter->GetInstigator();

    //for (int32 i = 0; i < ProjectileCount; ++i)
    //{
    //    // 월드 기준 -Y 방향을 0도로 시작 (왼쪽)
    //    float AngleDeg = 0.0f + AngleStep * i;
    //    float Rad = FMath::DegreesToRadians(AngleDeg);

    //    // 언리얼은 X가 앞, Y가 오른쪽 → -Y가 왼쪽
    //    FVector BaseDirection = FVector(FMath::Sin(Rad), -FMath::Cos(Rad), 0.0f);  // 0도 = -Y

    //    // 위치: 중심에서 방향 * 거리 + Z 오프셋
    //    FVector SpawnLocation = CenterLocation + BaseDirection * 50.0f + FVector(0.0f, 0.0f, MuzzleOffset.Z);
    //    FRotator SpawnRotation = BaseDirection.Rotation();

    //    FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

    //    APPProjectileBase* ProjectileObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<APPProjectileBase>(ProjectileClass, SpawnTransform);
    //    if (ProjectileObject)
    //    {
    //        ProjectileObject->InitProjectile(UPPGameSingleton::Get().SkillDatas[Type]);
    //        ProjectileObject->FinishSpawning(SpawnTransform);
    //    }

    //}
    


}

int32 UPPProjectileSkill::GetLevelBasedProjectileCount() const
{
    return 1 + SkillLevel;
}

