// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPSkillBase.h"
#include "Skill/PPSkillData.h"

// Sets default values
UPPSkillBase::UPPSkillBase()
{

}

void UPPSkillBase::SetData(FPPSkillData InSkillData, int32 InSlotIndex)
{
    bIsAuto = InSkillData.bIsAuto;
    SlotIndex = InSlotIndex;
    Cooldown = InSkillData.Cooldown;
}

void UPPSkillBase::TickSkill(float DeltaTime)
{
    //if (!bIsAuto)
    //{
    //    float Ratio = GetCooldownRatio();
    //    OnCooldownUpdated.Broadcast(SlotIndex, Ratio); // 갱신 알림
    //}
    //else
    //{
    //    TryUseSkill(); // 자동 사용 시도
    //}

    if (bIsAuto)
    {
        TryUseSkill(); // 자동 사용 시도
    }
    float Ratio = GetCooldownRatio();
    if (OnCooldownUpdated.IsBound())
    {
        OnCooldownUpdated.Broadcast(SlotIndex, Ratio); // 갱신 알림
    }
}

bool UPPSkillBase::TryUseSkill()
{
    if (CanUseSkill())
    {
        UseSkill();
        LastUsedTime = GetWorld()->GetTimeSeconds();
        return true;
    }
    return false;
}


void UPPSkillBase::UseSkill()
{
}


bool UPPSkillBase::CanUseSkill() const
{
    const UWorld* World = GetWorld();
    if (!World) return false;

    return (World->GetTimeSeconds() - LastUsedTime) >= Cooldown;
}

float UPPSkillBase::GetCooldownRatio() const
{
    const UWorld* World = GetWorld();
    if (!World) return 0.0f;

    float Elapsed = World->GetTimeSeconds() - LastUsedTime;
    return FMath::Clamp(1.0f - Elapsed / Cooldown, 0.0f, 1.0f);
}