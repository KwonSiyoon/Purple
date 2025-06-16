// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/PPPlayerSkillType.h"
#include "PPSelectSkillWidget.generated.h"


USTRUCT(BlueprintType)
struct FSelectableSkillData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlayerSkillType SkillType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NextLevel;
};

class UButton;
class UImage;
class UPPPlayerController;

/**
 * 
 */
UCLASS()
class PURPLE_API UPPSelectSkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void Init(class APPPlayerController* InOwnerController);
    void RefreshSelectableSkills();
    void ApplySelectedSkill(int32 Index);

protected:
    virtual void NativeConstruct() override;

    /** 선택 가능한 스킬 후보들 */
    UPROPERTY()
    TArray<FSelectableSkillData> CurrentChoices;

    /** 최대 선택지 수 */
    static constexpr int32 MaxChoices = 3;

    UPROPERTY()
    TObjectPtr<APPPlayerController> OwnerController;

    /** 선택된 스킬의 버튼을 바인딩할 때 사용할 함수 */
    UFUNCTION()
    void OnSkillCardClicked(int32 Index);
	
};
