// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/PPStageManager.h"
#include "Stage/PPWaveSpawnManager.h"

// Sets default values
APPStageManager::APPStageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WaveManager = CreateDefaultSubobject<UPPWaveSpawnManager>(TEXT("WaveSpawnManager"));
}

void APPStageManager::StartStage()
{
    if (WaveManager)
    {
        WaveManager->ResetWaves(); // 0초부터 시작
        UE_LOG(LogTemp, Log, TEXT("StartStage"));
    }
}

// Called when the game starts or when spawned
void APPStageManager::BeginPlay()
{
	Super::BeginPlay();
    if (WaveManager && StageWaveTable && SpawnPatternTable)
    {
        WaveManager->Initialize(StageWaveTable, SpawnPatternTable);
        StartStage(); // 자동 시작
    }
}

// Called every frame
void APPStageManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (WaveManager)
    {
        WaveManager->TickWave(DeltaTime);
    }
}

