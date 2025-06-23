// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPSelectSkillWidget.h"
#include "UI/PPCardWidget.h"
#include "Player/PPPlayerController.h"
#include "Skill/PPSkillData.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/PPGameSingleton.h"
#include "Algo/RandomShuffle.h"
#include "Skill/PPSkillBase.h"

void UPPSelectSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkillCard_0)
	{
		SkillCard_0->GetSelectButton()->OnClicked.AddDynamic(this, &UPPSelectSkillWidget::OnSkillCardClicked_0);
	}

	if (SkillCard_1)
	{
		SkillCard_1->GetSelectButton()->OnClicked.AddDynamic(this, &UPPSelectSkillWidget::OnSkillCardClicked_1);
	}
}


void UPPSelectSkillWidget::Init(APPPlayerController* InOwnerController)
{
	OwnerController = InOwnerController;
	RefreshSelectableSkills();
}

void UPPSelectSkillWidget::RefreshSelectableSkills()
{
	if (!OwnerController || !OwnerController->GetSkillDataTable()) return;

	CurrentSkillData.Empty();

	// 스킬 후보군 모으기
	TArray<EPlayerSkillType> AllSkillTypes;
	for (uint8 i = 0; i < static_cast<uint8>(EPlayerSkillType::Max); ++i)
	{
		AllSkillTypes.Add(static_cast<EPlayerSkillType>(i));
	}

	// 랜덤 셔플
	Algo::RandomShuffle(AllSkillTypes);

	int32 SelectedCount = 0;

	for (EPlayerSkillType SkillType : AllSkillTypes)
	{
		if (SelectedCount >= 2)
			break;

		int32 CurrentLevel = 0;
		bool bOwned = false;

		// 보유 여부 판단
		if (OwnerController->OwnedSkills.Contains(SkillType))
		{
			if (UPPSkillBase* Skill = OwnerController->OwnedSkills[SkillType])
			{
				CurrentLevel = Skill->GetCurrentLevel();
				bOwned = true;
			}
		}

		int32 NextLevel = CurrentLevel + 1;

		// Max 레벨 초과면 제외
		const FPPSkillData* MaxLevelData = UPPGameSingleton::Get().GetSkillData(SkillType, NextLevel);
		if (!MaxLevelData)
			continue;
		CurrentSkillData.Add(*MaxLevelData);
		++SelectedCount;
	}

	// 카드 1, 2에 각각 표시
	if (SkillCard_0 && CurrentSkillData.Num() > 0)
	{
		SkillCard_0->SetSkillData(CurrentSkillData[0]);
	}
	if (SkillCard_1 && CurrentSkillData.Num() > 1)
	{
		SkillCard_1->SetSkillData(CurrentSkillData[1]);
	}
}

void UPPSelectSkillWidget::ApplySelectedSkill(int32 Index)
{
    if (!OwnerController || !CurrentSkillData.IsValidIndex(Index)) return;

    OwnerController->AcquireSkill(CurrentSkillData[Index].SkillType);
    RemoveFromParent();

    // 게임 재개
    OwnerController->SetPause(false);
    OwnerController->SetShowMouseCursor(false);
    FInputModeGameOnly InputMode;
    OwnerController->SetInputMode(InputMode);

    // 중복 방지 플래그 해제
    OwnerController->bIsSkillUIOpen = false;

    // 누적된 레벨업이 있다면 다시 UI 띄우기
    if (OwnerController->PendingSkillUpCount > 0)
    {
        --OwnerController->PendingSkillUpCount;
        OwnerController->ShowSkillSelectUI(); // 재귀적 호출
    }
}

void UPPSelectSkillWidget::OnSkillCardClicked_0()
{
    ApplySelectedSkill(0);
}

void UPPSelectSkillWidget::OnSkillCardClicked_1()
{
    ApplySelectedSkill(1);
}