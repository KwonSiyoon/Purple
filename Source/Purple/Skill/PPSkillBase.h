// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PPPlayerSkillType.h"
#include "PPSkillBase.generated.h"


UCLASS()
class PURPLE_API UPPSkillBase : public UObject
{
	GENERATED_BODY()
	
public:	
	UPPSkillBase();

	virtual void Initialize(class APPCharacterBase* InOwner) { OwnerCharacter = InOwner; }

	UFUNCTION(BlueprintCallable)
	bool TryUseSkill();

	bool CanUseSkill() const;

	float GetCooldownRatio() const;

	UFUNCTION(BlueprintCallable)
	virtual void UseSkill();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	EPlayerSkillType SkillType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float Cooldown = 0.0f;

protected:
	UPROPERTY()
	TObjectPtr<class APPCharacterBase> OwnerCharacter;

	float LastUsedTime = -FLT_MAX;
};
