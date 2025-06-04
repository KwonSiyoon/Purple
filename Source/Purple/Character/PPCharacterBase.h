// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Skill/PPProjectileSkill.h" // UPPProjectileSkill 정의
#include "Skill/PPPlayerSkillType.h"       // EPlayerSkillType 정의
#include "PPCharacterBase.generated.h"



UENUM()
enum class ECharacterControlType : uint8
{
	Horse,
	Quarter
};

UCLASS()
class PURPLE_API APPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPCharacterBase();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void QuarterMove(const FInputActionValue& Value);

protected:	// Camera Section.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;


protected:	// Input Section.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuarterMoveAction;

	ECharacterControlType CurrentCharacterControlType;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UPPCharacterControlData*> CharacterControlManager;

public:	// Skill Section.
	// 캐릭터가 보유한 스킬들
	UPROPERTY()
	TMap<EPlayerSkillType, TObjectPtr<UPPSkillBase>> OwnedSkills;

	// 스킬 클래스와 발사체 블루프린트를 설정하기 위한 UPROPERTY
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UPPProjectileSkill> ProjectileSkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class APPProjectileBase> ProjectileBPClass;

	void UseActiveSkill(EPlayerSkillType SkillType);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetCharacterControlData(const class UPPCharacterControlData* InCharacterControlData);

	void SetCharacterControl(ECharacterControlType NewCharacterControlType);


};
