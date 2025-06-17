// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "UI/PPHUDWidget.h"
#include "Skill/PPSkillData.h"
#include "Engine/DataTable.h"
#include "UI/PPEquippedSkillWidget.h"

APPPlayerController::APPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPPHUDWidget> PPHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Purple/UI/WBP_InGame.WBP_InGame_C'"));
	if (PPHUDWidgetRef.Class)
	{
		PPHUDWidgetClass = PPHUDWidgetRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Purple/SkillData/SkillDataTable.SkillDataTable'"));
	if (SkillDataTableRef.Object)
	{
		SkillDataTable = SkillDataTableRef.Object;
	}

}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameInput;

	SetInputMode(GameInput);

	// 위젯 생성.
	PPHUDWidget = CreateWidget<UPPHUDWidget>(this, PPHUDWidgetClass);

	// 위젯이 문제 없이 생성 됐으면,
	if (PPHUDWidget)
	{
		// 위젯을 화면에 추가해 UI가 보일 수 있도록 설정.
		PPHUDWidget->AddToViewport();
	}

	//AcquireSkill(EPlayerSkillType::Empty, 0);

}

void APPPlayerController::AcquireSkill(EPlayerSkillType SkillType, int32 SlotIndex = 0)
{
	if (!SkillDataTable || !PPHUDWidget || !PPHUDWidget->GetEquippedSkillWidget()) return;

	const FString EnumString = StaticEnum<EPlayerSkillType>()->GetNameStringByValue((int64)SkillType); // "EPlayerSkillType::Fireball"
	const FString RowString = EnumString.Replace(TEXT("EPlayerSkillType::"), TEXT("")); // "Fireball"
	const FName RowName = FName(*RowString);
	const FPPSkillData* SkillData = SkillDataTable->FindRow<FPPSkillData>(RowName, TEXT("AssignSkill"));

	

	if (SkillData)
	{
		PPHUDWidget->GetEquippedSkillWidget()->AssignSkillToSlot(SlotIndex, SkillType, SkillData->Icon);
		UE_LOG(LogTemp, Log, TEXT("SkillData 가져옴."));
	}

	//// 스킬 획득 시
	//const FPPSkillData* SkillData = SkillDataTable->FindRow<FPPSkillData>(RowName, TEXT("AssignSkill"));
	//if (SkillData && HUD && HUD->GetEquippedSkillWidget())
	//{
	//	HUD->GetEquippedSkillWidget()->AssignSkillToSlot(SlotIndex, *SkillData);
	//}
}
