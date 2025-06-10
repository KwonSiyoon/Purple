// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPDropComponent.h"
#include "Collectible/ExpOrbActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPPDropComponent::UPPDropComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<AExpOrbActor> ExpOrbRef(TEXT("/Game/Purple/Blueprint/BP_ExpOrb.BP_ExpOrb_C"));
    if (ExpOrbRef.Succeeded())
    {
        ExpOrbClass = ExpOrbRef.Class;
        UE_LOG(LogTemp, Log, TEXT("ExpOrbRef 존재."));
    }
}

void UPPDropComponent::SpawnExp(float ExpValue, AActor* TargetPlayer)
{
    UE_LOG(LogTemp, Log, TEXT("UPPDropComponent - SpawnExp 들어옴."));
    if (!ExpOrbClass) return;

    FVector Origin = GetOwner()->GetActorLocation();

    const int32 OrbCount = FMath::Clamp(FMath::RoundToInt(ExpValue / 10.f), 1, 5);
    for (int32 i = 0; i < OrbCount; ++i)
    {
        // XY 평면에서만 랜덤
        FVector2D Rand2D = FMath::RandPointInCircle(100.f);
        FVector Offset(Rand2D.X, Rand2D.Y, 0.0f);

        FVector SpawnLoc = Origin + Offset;

        FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLoc);

        AExpOrbActor* Orb = GetWorld()->SpawnActorDeferred<AExpOrbActor>(ExpOrbClass, SpawnTransform, GetOwner());
        if (Orb)
        {
            Orb->Init(ExpValue / OrbCount, TargetPlayer);
            Orb->FinishSpawning(SpawnTransform);
        }
    }
}


// Called when the game starts
void UPPDropComponent::BeginPlay()
{
	Super::BeginPlay();
}


