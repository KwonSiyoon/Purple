// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/PPSkillBase.h"
#include "PPProjectileSkill.generated.h"

/**
 * 
 */
UCLASS()
class PURPLE_API APPProjectileSkill : public APPSkillBase
{
	GENERATED_BODY()

public:
	APPProjectileSkill();

	virtual void UseSkill() override;


protected:

	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class USphereComponent> CollisionComp;

	// 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	// 나이아가라 이펙트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UNiagaraComponent> NiagaraEffect;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

};
