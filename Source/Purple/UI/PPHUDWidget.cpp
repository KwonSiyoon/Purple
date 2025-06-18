// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPHUDWidget.h"
#include "PPEquippedSkillWidget.h"
#include "Components/ProgressBar.h"

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

void UPPHUDWidget::UpdateExpBar(float Ratio)
{
	if (ExpBar)
	{
		ExpBar->SetPercent(Ratio);
	}
}


void UPPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EquippedSkillWidget = Cast<UPPEquippedSkillWidget>(GetWidgetFromName(TEXT("EquippedSkill")));

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EXP_Bar")));

}
