// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/PPPlayerSkillType.h"
#include "PPEquippedSkillWidget.generated.h"


USTRUCT(BlueprintType)
struct FEquippedSkillSlotUI
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UProgressBar> CooldownBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;

	/** 할당된 스킬 타입 */
	EPlayerSkillType AssignedSkill = EPlayerSkillType::Empty;

	/** 현재 쿨타임 */
	float Cooldown = 0.f;
};


/**
 * 
 */
UCLASS()
class PURPLE_API UPPEquippedSkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPEquippedSkillWidget(const FObjectInitializer& ObjectInitializer);

	// UI 슬롯 구조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill UI")
	TArray<FEquippedSkillSlotUI> SkillSlots;

	// 스킬 타입 → 슬롯 인덱스 찾기
	int32 FindSlotBySkillType(EPlayerSkillType SkillType) const;

	// 스킬 획득 시 슬롯에 할당 
	void AssignSkillToSlot(int32 Index, EPlayerSkillType SkillType, UTexture2D* Icon);
	void AssignSkillToSlot(int32 Index, const struct FPPSkillData* InSkillData);

	// 쿨타임 갱신 
	void UpdateCooldown(EPlayerSkillType SkillType, float Ratio);
	void UpdateCooldown(int32 Index, float Ratio);

	void BindSkill(class UPPSkillBase* Skill, int32 SlotIndex);
	void UpdateCooldownBar(float Ratio);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



};
