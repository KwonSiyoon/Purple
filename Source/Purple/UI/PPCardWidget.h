// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PPUserWidget.h"
#include "Skill/PPSkillData.h"
#include "PPCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class PURPLE_API UPPCardWidget : public UPPUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetSkillData(const FPPSkillData& InSkillData);

	class UButton* GetSelectButton() const;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBox_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Description;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Damage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ProjectileAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SelectButton;
};
