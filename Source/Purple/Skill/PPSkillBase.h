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
	virtual void UseSkill();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	EPlayerSkillType SkillType;

protected:
	UPROPERTY()
	TObjectPtr<class APPCharacterBase> OwnerCharacter;

};
