// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "UI/PPHUDWidget.h"
#include "Skill/PPSkillData.h"
#include "Skill/PPSkillBase.h"
#include "Engine/DataTable.h"
#include "UI/PPEquippedSkillWidget.h"
#include "GameData/PPGameSingleton.h"
#include "UI/PPSelectSkillWidget.h"
#include "Character/PPCharacterBase.h"
#include "Skill/PPProjectileSkill.h"
#include "Projectile/PPProjectileBase.h"


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

	static ConstructorHelpers::FClassFinder<UPPSelectSkillWidget> SelectWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Purple/UI/WBP_SelectSkill.WBP_SelectSkill_C'"));
	if (SelectWidgetRef.Succeeded())
	{
		SelectSkillWidgetClass = SelectWidgetRef.Class;
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


}

void APPPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (const auto& SkillPair : OwnedSkills)
	{
		if (SkillPair.Value)
		{
			SkillPair.Value->TickSkill(DeltaTime);
		}
	}
}

void APPPlayerController::AcquireSkill(EPlayerSkillType SkillType, int32 SlotIndex)
{

	if (!PlayerCharacter) return;

	// 스킬 레벨 확인 (Character가 관리)
	//int32 NewSkillLevel = PlayerCharacter->GetSkillLevel(SkillType) + 1;

	const FPPSkillData* SkillData = UPPGameSingleton::Get().GetSkillData(SkillType, 1);
	if (!SkillData) return;

	// 스킬 인스턴스 생성
	UPPProjectileSkill* NewSkill = NewObject<UPPProjectileSkill>(this, UPPProjectileSkill::StaticClass());
	NewSkill->Initialize(PlayerCharacter);
	NewSkill->SetProjectileClass(APPProjectileBase::StaticClass(), SkillType);
	NewSkill->SetData(*SkillData, SlotIndex);

	// 등록 및 바인딩
	OwnedSkills.Add(SkillType, NewSkill);
	BindSkill(NewSkill, SlotIndex);
	PPHUDWidget->GetEquippedSkillWidget()->AssignSkillToSlot(SlotIndex, SkillData);

	// 캐릭터는 내부 상태만 저장
	PlayerCharacter->AcquireSkill(SkillType, SlotIndex);

}

void APPPlayerController::UpdateExp(float Ratio)
{
	PPHUDWidget->UpdateExpBar(Ratio);
}

void APPPlayerController::BindSkill(UPPSkillBase* Skill, int32 SlotIndex)
{
	PPHUDWidget->GetEquippedSkillWidget()->BindSkill(Skill, SlotIndex);
}

UDataTable* APPPlayerController::GetSkillDataTable() const
{
	return SkillDataTable;
}

void APPPlayerController::ShowSkillSelectUI()
{
	if (!SelectSkillWidgetClass) return;

	if (bIsSkillUIOpen)
	{
		++PendingSkillUpCount;
		return;
	}

	UPPSelectSkillWidget* SelectWidget = CreateWidget<UPPSelectSkillWidget>(this, SelectSkillWidgetClass);
	if (SelectWidget)
	{
		SelectWidget->AddToViewport();
		SelectWidget->Init(this);

		// 상태 설정
		bIsSkillUIOpen = true;

		SetPause(true);
		SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(SelectWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
	}
}
