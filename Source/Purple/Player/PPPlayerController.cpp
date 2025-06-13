// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "UI/PPUserWidget.h"
#include "Skill/PPSkillData.h"
#include "Engine/DataTable.h"

APPPlayerController::APPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPPUserWidget> PPUserWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Purple/UI/WBP_InGame.WBP_InGame_C'"));
	if (PPUserWidgetRef.Class)
	{
		PPUserWidgetClass = PPUserWidgetRef.Class;
	}

	//// 스킬 획득 시
	//const FPPSkillData* SkillData = SkillDataTable->FindRow<FPPSkillData>(RowName, TEXT("AssignSkill"));
	//if (SkillData && HUD && HUD->GetEquippedSkillWidget())
	//{
	//	HUD->GetEquippedSkillWidget()->AssignSkillToSlot(SlotIndex, *SkillData);
	//}

}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameInput;

	SetInputMode(GameInput);

	// 위젯 생성.
	PPUserWidget = CreateWidget<UPPUserWidget>(this, PPUserWidgetClass);

	// 위젯이 문제 없이 생성 됐으면,
	if (PPUserWidget)
	{
		// 위젯을 화면에 추가해 UI가 보일 수 있도록 설정.
		PPUserWidget->AddToViewport();
	}

}
