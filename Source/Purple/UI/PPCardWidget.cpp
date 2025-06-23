// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPCardWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UPPCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPPCardWidget::SetSkillData(const FPPSkillData& InSkillData)
{
	if (Image_Icon)
	{
		Image_Icon->SetBrushFromTexture(InSkillData.Icon);
	}
	if (TextBox_Name)
	{
		TextBox_Name->SetText(InSkillData.DisplayName);
	}
	if (Description)
	{
		Description->SetText(InSkillData.Description);
	}
	if (Level)
	{
		Level->SetText(FText::Format(FText::FromString(TEXT("Level : {0}")), InSkillData.Level));
	}
	if (Damage)
	{
		Damage->SetText(FText::Format(FText::FromString(TEXT("Damage : {0}")), FText::AsNumber(InSkillData.Damage)));
	}
	if (ProjectileAmount)
	{
		ProjectileAmount->SetText(FText::Format(FText::FromString(TEXT("Projectile Amount : {0}")), FText::AsNumber(InSkillData.ProjectileAmount)));
	}
}

UButton* UPPCardWidget::GetSelectButton() const
{
	return SelectButton;
}
