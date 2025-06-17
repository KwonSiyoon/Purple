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
	void AcquireSkill(EPlayerSkillType SkillType, int32 SlotIndex);
	class  UPPHUDWidget* GetHUDWidget() { return PPHUDWidget; }
	/*void UpdateCoolDown();*/

protected:	// HUD Section.
	// 생성할 위젯의 클래스 정보.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UPPHUDWidget> PPHUDWidgetClass;

	// 생성한 위젯의 객체 정보를 저장할 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<class UPPHUDWidget> PPHUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> SkillDataTable;


};
