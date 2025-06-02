// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PPProjectileSkill.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/PPEnemyCharacterBase.h"
#include "Engine/DamageEvents.h"

APPProjectileSkill::APPProjectileSkill()
{
    // 충돌 컴포넌트
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(10.0f);
    CollisionComp->SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APPProjectileSkill::OnSphereBeginOverlap);
    RootComponent = CollisionComp;

    // 이동
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    // 나이아가라 이펙트
    NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
    NiagaraEffect->SetupAttachment(RootComponent);
    NiagaraEffect->SetRelativeLocation(FVector(-27.0f, 0.0f, 0.0f));
}

void APPProjectileSkill::UseSkill()
{
}

void APPProjectileSkill::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APPEnemyCharacterBase* EnemyPawn = Cast<APPEnemyCharacterBase>(OtherActor);
    if (EnemyPawn)
    {
        FDamageEvent DamageEvent;

        //EnemyPawn->TakeDamage(0.0f, DamageEvent, GetWorld()->GetPlayerControllerIterator)
    }
}
