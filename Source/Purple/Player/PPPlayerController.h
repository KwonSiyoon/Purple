// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Skill/PPPlayerSkillType.h"
#include "PPPlayerController.generated.h"


class UDataTable;
struct FPPSkillData;

/**
 * 
 */
UCLASS()
class PURPLE_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APPPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void AcquireSkill(EPlayerSkillType SkillType, int32 SlotIndex = 0);
	void UpdateExp(float Ratio);
	FORCEINLINE class  UPPHUDWidget* GetHUDWidget() { return PPHUDWidget; }
	FORCEINLINE void SetPlayerCharacter(class APPCharacterBase* InPlayerCharacter)
	{
		PlayerCharacter = InPlayerCharacter;
	}
	FORCEINLINE class APPCharacterBase* GetPlayerCharacter() { return PlayerCharacter; }

	void BindSkill(class UPPSkillBase* Skill, int32 SlotIndex);
	UDataTable* GetSkillDataTable() const;
	void ShowSkillSelectUI();
	/*void UpdateCoolDown();*/

protected:	// HUD Section.
	// 생성할 위젯의 클래스 정보.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UPPHUDWidget> PPHUDWidgetClass;

	// 생성한 위젯의 객체 정보를 저장할 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<class UPPHUDWidget> PPHUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPPSelectSkillWidget> SelectSkillWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> SkillDataTable;

	
public:
	UPROPERTY()
	bool bIsSkillUIOpen = false;

	UPROPERTY()
	int32 PendingSkillUpCount = 0;

	// 캐릭터가 보유한 스킬들
	UPROPERTY()
	TMap<EPlayerSkillType, TObjectPtr<class UPPSkillBase>> OwnedSkills;

	// 스킬 클래스와 발사체 블루프린트를 설정하기 위한 UPROPERTY
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class UPPProjectileSkill> ProjectileSkillClass;

	UPROPERTY()
	TObjectPtr<class APPCharacterBase> PlayerCharacter;

};
