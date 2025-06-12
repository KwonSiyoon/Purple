// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PPEnemyAIInterface.h"
#include "Interface/PPAnimationAttackInterface.h"
#include "PPEnemyCharacterBase.generated.h"

UCLASS()
class PURPLE_API APPEnemyCharacterBase : public ACharacter, public IPPEnemyAIInterface, public IPPAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPEnemyCharacterBase();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	// 콤보가 종료될 때 호출될 함수.
	// 애님 몽타주에서 제공하는 델리게이트와 파라미터 맞춤.
	void AttackActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void SetDead();
	void PlayDeadAnimation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Inherited via IPPEnemyAIInterface
	float GetAIAttackRange() override;

	float GetAITurnSpeed() override;

	void AttackByAI() override;
	virtual void SetAIAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished);
	FAIEnemyAttackFinished OnAttackFinished;

	// Inherited via IPPAnimationAttackInterface
	void AttackHitCheck() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;


	float ExpRewardAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drop", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPPDropComponent> DropComponent;

protected:
	/** RVO 회피 반경(동일 적군과 일정 거리 유지) */
	UPROPERTY(EditDefaultsOnly, Category = "AI|Avoidance", meta = (ClampMin = "50.0"))
	float AvoidanceRadius = 150.f;    // 기본값: 150 cm

};
