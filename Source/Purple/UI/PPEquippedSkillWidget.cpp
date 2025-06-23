// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPEquippedSkillWidget.h"
#include "Components/ProgressBar.h"
#include "Styling/SlateTypes.h"
#include "Character/PPCharacterBase.h"
#include "Skill/PPSkillBase.h"
#include "Skill/PPSkillData.h"

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
	FEquippedSkillSlotUI& SlotUI = SkillSlots[Index];
	SlotUI.AssignedSkill = SkillType;
	SlotUI.Icon = Icon;
	UE_LOG(LogTemp, Log, TEXT("AssignSkillToSlot 들어옴."));
	FSlateBrush Brush;
	if (SlotUI.CooldownBar && Icon)
	{
		
		Brush.SetResourceObject(Icon);
		FProgressBarStyle ProgressBarStyle = SlotUI.CooldownBar->GetWidgetStyle();
		ProgressBarStyle.SetBackgroundImage(Brush);
		SlotUI.CooldownBar->SetWidgetStyle(ProgressBarStyle);
		UE_LOG(LogTemp, Log, TEXT("SkillData 등록."));

	}
	else if(SlotUI.CooldownBar && !Icon)
	{
		FProgressBarStyle ProgressBarStyle = SlotUI.CooldownBar->GetWidgetStyle();
		ProgressBarStyle.SetBackgroundImage(Brush);
		SlotUI.CooldownBar->SetWidgetStyle(ProgressBarStyle);
		UE_LOG(LogTemp, Log, TEXT("SkillData 등록 없음."));
	}
}

void UPPEquippedSkillWidget::AssignSkillToSlot(int32 Index, const FPPSkillData* InSkillData)
{
	if (!SkillSlots.IsValidIndex(Index)) return;
	FEquippedSkillSlotUI& SlotUI = SkillSlots[Index];
	SlotUI.AssignedSkill = InSkillData->SkillType;
	SlotUI.Icon = InSkillData->Icon;
	UE_LOG(LogTemp, Log, TEXT("AssignSkillToSlot 들어옴."));
	FSlateBrush Brush;
	if (SlotUI.CooldownBar && SlotUI.Icon)
	{

		Brush.SetResourceObject(SlotUI.Icon);
		FProgressBarStyle ProgressBarStyle = SlotUI.CooldownBar->GetWidgetStyle();
		ProgressBarStyle.SetBackgroundImage(Brush);
		SlotUI.CooldownBar->SetWidgetStyle(ProgressBarStyle);
		UE_LOG(LogTemp, Log, TEXT("SkillData 등록."));

	}
	else if (SlotUI.CooldownBar && !SlotUI.Icon)
	{
		FProgressBarStyle ProgressBarStyle = SlotUI.CooldownBar->GetWidgetStyle();
		ProgressBarStyle.SetBackgroundImage(Brush);
		SlotUI.CooldownBar->SetWidgetStyle(ProgressBarStyle);
		UE_LOG(LogTemp, Log, TEXT("SkillData 등록 없음."));
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

void UPPEquippedSkillWidget::UpdateCooldown(int32 Index, float Ratio)
{
	if (SkillSlots.IsValidIndex(Index) && SkillSlots[Index].CooldownBar)
	{
		SkillSlots[Index].CooldownBar->SetPercent(Ratio);
		SkillSlots[Index].Cooldown = Ratio;
	}
}

void UPPEquippedSkillWidget::BindSkill(UPPSkillBase* Skill, int32 SlotIndex)
{
	UE_LOG(LogTemp, Log, TEXT("%s() called."), TEXT(__FUNCTION__));
	if (Skill)
	{
		Skill->OnCooldownUpdated.AddUObject(this, &UPPEquippedSkillWidget::UpdateCooldown);
	}
}

void UPPEquippedSkillWidget::UpdateCooldownBar(float Ratio)
{

}

void UPPEquippedSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillSlots.SetNum(2); // 또는 정확한 슬롯 수

	SkillSlots[0].CooldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill_01")));
	SkillSlots[1].CooldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill_02")));
	
}

void UPPEquippedSkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	/*for (int32 i = 0; i < SkillSlots.Num(); ++i)
	{
		if (SkillSlots[i].AssignedSkill == EPlayerSkillType::Empty)
			continue;

		if (APlayerController* PC = GetOwningPlayer())
		{
			if (const ACharacter* Char = Cast<ACharacter>(PC->GetPawn()))
			{
				if (const APPCharacterBase* Player = Cast<APPCharacterBase>(Char))
				{
					const TObjectPtr<UPPSkillBase>* Found = Player->GetSkillByType(SkillSlots[i].AssignedSkill);
					if (Found && *Found)
					{
						const float Ratio = (*Found)->GetCooldownRatio();
						UpdateCooldown(i, Ratio);
					}
				}
			}
		}
	}*/
}


