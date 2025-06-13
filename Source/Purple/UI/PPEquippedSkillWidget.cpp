// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPEquippedSkillWidget.h"
#include "Components/ProgressBar.h"

UPPEquippedSkillWidget::UPPEquippedSkillWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 UPPEquippedSkillWidget::FindSlotBySkillType(EPlayerSkillType SkillType) const
{
	for (int32 i = 0; i < SkillSlots.Num(); ++i)
	{
		if (SkillSlots[i].AssignedSkill == SkillType)
			return i;
	}
	return INDEX_NONE;
}

void UPPEquippedSkillWidget::AssignSkillToSlot(int32 Index, EPlayerSkillType SkillType, UTexture2D* Icon)
{
	if (!SkillSlots.IsValidIndex(Index)) return;

	FEquippedSkillSlotUI& Slot = SkillSlots[Index];
	Slot.AssignedSkill = SkillType;
	Slot.Icon = Icon;

	if (Slot.CooldownBar && Icon)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Icon);
		Brush.ImageSize = FVector2D(64.f, 64.f);
		Slot.CooldownBar->WidgetStyle.BackgroundImage = Brush;
	}
}

void UPPEquippedSkillWidget::UpdateCooldown(EPlayerSkillType SkillType, float Ratio)
{
	int32 Index = FindSlotBySkillType(SkillType);
	if (SkillSlots.IsValidIndex(Index) && SkillSlots[Index].CooldownBar)
	{
		SkillSlots[Index].CooldownBar->SetPercent(Ratio);
		SkillSlots[Index].Cooldown = Ratio;
	}
}

void UPPEquippedSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillSlots.SetNum(2); // 또는 정확한 슬롯 수

	SkillSlots[0].CooldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill_01")));
	SkillSlots[1].CooldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill_02")));

	/*Skill_1 = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill_01")));
	Skill_2 = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill_02")));*/
	
}


