// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPHUDWidget.h"
#include "PPEquippedSkillWidget.h"

UPPHUDWidget::UPPHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UPPEquippedSkillWidget* UPPHUDWidget::GetEquippedSkillWidget()
{
	return EquippedSkillWidget;
}

void UPPHUDWidget::UpdateSkillCooldown(EPlayerSkillType SkillType, float Ratio)
{
	if (EquippedSkillWidget)
	{
		EquippedSkillWidget->UpdateCooldown(SkillType, Ratio);
	}
}


void UPPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EquippedSkillWidget = Cast<UPPEquippedSkillWidget>(GetWidgetFromName(TEXT("EquippedSkill")));
}
