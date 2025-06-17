// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/PPPlayerSkillType.h"
#include "PPHUDWidget.generated.h"

class UPPEquippedSkillWidget;


/**
 * 
 */
UCLASS()
class PURPLE_API UPPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPHUDWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateSkillCooldown(EPlayerSkillType SkillType, float Ratio);

	UPPEquippedSkillWidget* GetEquippedSkillWidget();

	UPROPERTY()
	TObjectPtr<UPPEquippedSkillWidget> EquippedSkillWidget;

protected:
	virtual void NativeConstruct() override;


};
