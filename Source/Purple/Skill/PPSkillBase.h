// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PPPlayerSkillType.h"
#include "PPSkillBase.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillCooldownUpdated, int32 /*SlotIndex*/, float /*CooldownRatio*/);

UCLASS()
class PURPLE_API UPPSkillBase : public UObject
{
	GENERATED_BODY()
	
public:	
	UPPSkillBase();

	virtual void Initialize(class APPCharacterBase* InOwner) { OwnerCharacter = InOwner; }

	virtual void SetData(struct FPPSkillData InSkillData, int32 InSlotIndex);

	FOnSkillCooldownUpdated OnCooldownUpdated;

	void TickSkill(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	bool TryUseSkill();

	bool CanUseSkill() const;

	float GetCooldownRatio() const;

	UFUNCTION(BlueprintCallable)
	virtual void UseSkill();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	EPlayerSkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float Cooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	bool bIsAuto;

	int32 SlotIndex;

protected:
	UPROPERTY()
	TObjectPtr<class APPCharacterBase> OwnerCharacter;

	float LastUsedTime = -FLT_MAX;
};
