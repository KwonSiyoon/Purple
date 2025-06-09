// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/PPProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/PPEnemyCharacterBase.h"
#include "Engine/DamageEvents.h"
#include "PPProjectileData.h"

// Sets default values
APPProjectileBase::APPProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 충돌 컴포넌트
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(10.0f);
    CollisionComp->SetCollisionProfileName("PlayerProjectile");
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APPProjectileBase::OnSphereBeginOverlap);
    RootComponent = CollisionComp;

    // 이동
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    // 나이아가라 이펙트
    TrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
    TrailEffect->SetupAttachment(RootComponent);
    TrailEffect->SetRelativeLocation(FVector(-27.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void APPProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APPEnemyCharacterBase* EnemyPawn = Cast<APPEnemyCharacterBase>(OtherActor);
    UE_LOG(LogTemp, Log, TEXT("Overlap 들어옴."));
    if (EnemyPawn)
    {
        UE_LOG(LogTemp, Log, TEXT("Overlap 들어옴 -> Enemy와 접촉."));
        FDamageEvent DamageEvent;

        //EnemyPawn->TakeDamage(0.0f, DamageEvent, GetWorld()->GetPlayerControllerIterator)
        AController* InstigatorController = SourceActor ? SourceActor->GetInstigatorController() : nullptr;

        float AppliedDamage = EnemyPawn->TakeDamage(Damage, DamageEvent, InstigatorController, this);

        // Impact 이펙트 출력
        if (ImpactEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                ImpactEffect,
                GetActorLocation()
            );
        }

        Destroy(); // 1회성 탄환
    }
}

void APPProjectileBase::InitProjectile(const FPPProjectileData& InData)
{
    Damage = InData.Damage;
    ImpactEffect = InData.ImpactEffect;
    SetLifeSpan(InData.LifeSpan);

    // 속도 초기화
    if (ProjectileMovement)
    {
        // 월드 기준 속도 적용
        ProjectileMovement->bInitialVelocityInLocalSpace = false;

        // 정확한 방향 적용 (SetActorRotation 이후 호출되므로 GetForwardVector가 정확해짐)
        FVector Direction = GetActorForwardVector().GetSafeNormal();
        ProjectileMovement->Velocity = Direction * InData.Speed;
    }

    // Trail Niagara 시작
    if (InData.TrailEffect)
    {
        TrailEffect->SetAsset(InData.TrailEffect);
        TrailEffect->Activate();
    }

    // 발사자 저장
    SourceActor = GetInstigator();
}

// Called every frame
void APPProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

