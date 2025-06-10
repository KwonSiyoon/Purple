// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible/ExpOrbActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"

AExpOrbActor::AExpOrbActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    RootComponent = Collision;
    Collision->InitSphereRadius(50.0f);
    Collision->SetCollisionProfileName(TEXT("Collectible"));

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Collision);
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Engine/BasicShapes/Sphere"));
    if (MeshRef.Succeeded())
    {
        Mesh->SetStaticMesh(MeshRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatRef(TEXT("/Script/Engine.Material'/Game/Purple/Blueprint/M_ExpOrb.M_ExpOrb'"));
    if (MatRef.Succeeded())
    {
        Mesh->SetMaterial(0, MatRef.Object);
    }
    Collision->OnComponentBeginOverlap.AddDynamic(this, &AExpOrbActor::OnOverlap);

}

void AExpOrbActor::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();

    UMaterialInterface* BaseMat = Mesh->GetMaterial(0);
    DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);
    ApplyOrbColorByExp();
}

void AExpOrbActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AccumulatedTime += DeltaTime;

    // 진동
    FVector Offset = FVector(0.0f, 0.0f, FMath::Sin(AccumulatedTime * FloatFrequency) * FloatAmplitude);
    SetActorLocation(StartLocation + Offset);

    // 흡수 이동
    if (TargetToFollow && FVector::Dist(GetActorLocation(), TargetToFollow->GetActorLocation()) < AbsorbDistance)
    {
        FVector Dir = (TargetToFollow->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorLocation(GetActorLocation() + Dir * AbsorbSpeed * DeltaTime);
    }
}

void AExpOrbActor::Init(float InExpValue, AActor* TargetPlayer)
{
    ExpValue = InExpValue;
    TargetToFollow = TargetPlayer;
}

void AExpOrbActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
    {
        // TODO: 경험치 전달
        UE_LOG(LogTemp, Log, TEXT("Exp %.1f 흡수됨."), ExpValue);

        // 파티클 효과 등 추가 가능

        Destroy();
    }
}

void AExpOrbActor::ApplyOrbColorByExp()
{
    if (!DynamicMaterial) return;

    FLinearColor Color;

    if (ExpValue < 10.0f)       Color = FLinearColor::Green;
    else if (ExpValue < 50.0f)  Color = FLinearColor::Yellow;
    else                        Color = FLinearColor::Red;

    DynamicMaterial->SetVectorParameterValue(FName("OrbColor"), Color);
}