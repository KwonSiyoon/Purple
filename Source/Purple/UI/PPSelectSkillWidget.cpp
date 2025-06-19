// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPSelectSkillWidget.h"
//#include "UI/PPSelectSkillCardWidget.h"
#include "Player/PPPlayerController.h"
#include "Skill/PPSkillData.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

void UPPSelectSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//// 바인딩: 각 카드 위젯이 OnClicked 시 OnSkillCardClicked 호출
	//for (int32 i = 0; i < SkillCardWidgets.Num(); ++i)
	//{
	//	if (SkillCardWidgets[i])
	//	{
	//		const int32 Index = i;
	//		SkillCardWidgets[i]->OnCardSelected.BindUObject(this, &UPPSelectSkillWidget::OnSkillCardClicked, Index);
	//	}
	//}
}


void UPPSelectSkillWidget::Init(APPPlayerController* InOwnerController)
{
	OwnerController = InOwnerController;
	RefreshSelectableSkills();
}

void UPPSelectSkillWidget::RefreshSelectableSkills()
{
	//if (!OwnerController || !OwnerController->GetSkillDataTable()) return;

	//CurrentChoices.Empty();

	//const TMap<EPlayerSkillType, int32>& OwnedSkills = OwnerController->GetOwnedSkills();
	//const UDataTable* SkillDataTable = OwnerController->GetSkillDataTable();

	//TArray<EPlayerSkillType> AllSkillTypes;
	//for (uint8 i = 0; i < static_cast<uint8>(EPlayerSkillType::Max); ++i)
	//{
	//	AllSkillTypes.Add(static_cast<EPlayerSkillType>(i));
	//}
	//AllSkillTypes.Shuffle();

	//for (EPlayerSkillType SkillType : AllSkillTypes)
	//{
	//	const int32 CurrentLevel = OwnedSkills.Contains(SkillType) ? OwnedSkills[SkillType] : 0;
	//	const FName RowName = *UEnum::GetValueAsString(SkillType).RightChop(19); // EPlayerSkillType:: 제거

	//	if (const FPPSkillData* SkillData = SkillDataTable->FindRow<FPPSkillData>(RowName, TEXT("")))
	//	{
	//		if (CurrentLevel < SkillData->LevelData.Num())
	//		{
	//			FSelectableSkillData SelectData;
	//			SelectData.SkillType = SkillType;
	//			SelectData.Icon = SkillData->Icon;
	//			SelectData.NextLevel = CurrentLevel + 1;
	//			CurrentChoices.Add(SelectData);

	//			if (CurrentChoices.Num() >= MaxChoices)
	//				break;
	//		}
	//	}
	//}

	//// 카드 위젯에 반영
	//for (int32 i = 0; i < SkillCardWidgets.Num(); ++i)
	//{
	//	if (!SkillCardWidgets[i]) continue;

	//	if (CurrentChoices.IsValidIndex(i))
	//	{
	//		SkillCardWidgets[i]->SetSkill(CurrentChoices[i]);
	//		SkillCardWidgets[i]->SetVisibility(ESlateVisibility::Visible);
	//	}
	//	else
	//	{
	//		SkillCardWidgets[i]->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//}
}

void UPPSelectSkillWidget::ApplySelectedSkill(int32 Index)
{
}

void UPPSelectSkillWidget::OnSkillCardClicked(int32 Index)
{
	if (!CurrentChoices.IsValidIndex(Index) || !OwnerController) return;

	const FSelectableSkillData& Chosen = CurrentChoices[Index];
	//OwnerController->AcquireSkill(Chosen.SkillType);

	RemoveFromParent(); // UI 닫기
}