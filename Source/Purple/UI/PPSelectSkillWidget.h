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

class UPPCardWidget;
class UButton;
class UImage;

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


    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPPCardWidget> SkillCard_0;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPPCardWidget> SkillCard_1;

    UPROPERTY()
    TObjectPtr<class APPPlayerController> OwnerController;

    UPROPERTY()
    TArray<struct FPPSkillData> CurrentSkillData; // 현재 표시 중인 스킬들

    UFUNCTION()
    void OnSkillCardClicked_0();

    UFUNCTION()
    void OnSkillCardClicked_1();
	
};
