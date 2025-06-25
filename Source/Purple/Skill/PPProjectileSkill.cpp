// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/PPProjectileSkill.h"

#include "Projectile/PPProjectileBase.h"
#include "Character/PPCharacterBase.h"
#include "GameData/PPGameSingleton.h"

#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/OverlapResult.h"


UPPProjectileSkill::UPPProjectileSkill()
{
    
}

void UPPProjectileSkill::SetProjectileClass(TSubclassOf<APPProjectileBase> InClass, EPlayerSkillType InType)
{
    ProjectileClass = InClass;
    Type = InType;
}


void UPPProjectileSkill::SetData(FPPSkillData InSkillData, int32 InSlotIndex)
{
	Super::SetData(InSkillData, InSlotIndex);
	ProjectileCount = InSkillData.ProjectileAmount;
}

void UPPProjectileSkill::UseSkill()
{
	if (!OwnerCharacter || !ProjectileClass) return;

	FVector CenterLocation = OwnerCharacter->GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(2000.0f); // 탐색 반경 조절 가능

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ProjectileSkillOverlap), false);
	QueryParams.AddIgnoredActor(OwnerCharacter);


	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		CenterLocation,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere,
		QueryParams
	);

	AActor* ClosestEnemy = nullptr;
	float ClosestDistSqr = TNumericLimits<float>::Max();

	for (FOverlapResult& Result : Overlaps)
	{
		ACharacter* Candidate = Cast<ACharacter>(Result.GetActor());
		if (!Candidate || Candidate == OwnerCharacter || !IsValid(Candidate)) continue;

		const float DistSqr = FVector::DistSquared(CenterLocation, Candidate->GetActorLocation());
		if (DistSqr < ClosestDistSqr)
		{
			ClosestDistSqr = DistSqr;
			ClosestEnemy = Candidate;
		}
	}

	// 2. 방향 설정: 가장 가까운 적 기준 (없으면 -Y)
	FVector StartDirection = FVector::BackwardVector; // 기본 방향: -Y
	if (ClosestEnemy)
	{
		StartDirection = (ClosestEnemy->GetActorLocation() - CenterLocation);
		StartDirection.Z = 0.0f;
		StartDirection.Normalize();
	}

	// 3. 발사체 생성
	const int32 InProjectileCount = ProjectileCount + (SkillLevel - 1) * ProjectilesPerLevel;
	const float AngleStep = 360.0f / FMath::Max(1, InProjectileCount);

	for (int32 i = 0; i < InProjectileCount; ++i)
	{
		const float AngleDeg = AngleStep * i;
		const FVector RotatedDirection = StartDirection.RotateAngleAxis(AngleDeg, FVector::UpVector);

		const FVector SpawnLocation = CenterLocation + RotatedDirection * 50.0f + FVector(0.0f, 0.0f, MuzzleOffset.Z);
		const FRotator SpawnRotation = FRotationMatrix::MakeFromX(RotatedDirection).Rotator();

		const FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		APPProjectileBase* ProjectileObject = GetWorld()->SpawnActorDeferred<APPProjectileBase>(
			ProjectileClass, SpawnTransform, OwnerCharacter, OwnerCharacter->GetInstigator()
		);

		if (ProjectileObject)
		{
			ProjectileObject->InitProjectile(UPPGameSingleton::Get().ProjectileDatas[Type]);
			ProjectileObject->FinishSpawning(SpawnTransform);
			ProjectileObject->SetActorRotation(SpawnRotation);
		}
	}
}

int32 UPPProjectileSkill::GetLevelBasedProjectileCount() const
{
    return 1 + SkillLevel;
}

