// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPSkillBase.h"


// Sets default values
UPPSkillBase::UPPSkillBase()
{

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
	UE_LOG(LogTemp, Log, TEXT("In UseSkill"))
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
    return FMath::Clamp(Elapsed / Cooldown, 0.0f, 1.0f);
}