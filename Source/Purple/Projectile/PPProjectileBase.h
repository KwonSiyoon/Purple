// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPProjectileBase.generated.h"



UCLASS()
class PURPLE_API APPProjectileBase : public AActor
{
	GENERATED_BODY()

	friend class APPProjectileSkill;
	
public:	
	// Sets default values for this actor's properties
	APPProjectileBase();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void InitProjectile(const class FPPProjectileData& InData);

protected:

	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class USphereComponent> CollisionComp;

	// 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	// 나이아가라 이펙트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UNiagaraComponent> TrailEffect;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UNiagaraSystem> ImpactEffect; // 스폰형 이펙트

	UPROPERTY()
	float Damage;

	UPROPERTY()
	TObjectPtr<AActor> SourceActor; // 발사자 정보 저장



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
