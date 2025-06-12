// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PPWaveSpawnManager.generated.h"

USTRUCT()
struct FWaveRuntime
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FName RowName;

    // 데이터테이블 상의 WaveRow를 참조
    const struct FPPStageWave* WaveData = nullptr;

    UPROPERTY()
    bool bFinished = false;

    UPROPERTY()
    float Accumulator = 0.f;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURPLE_API UPPWaveSpawnManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPPWaveSpawnManager();
    /** DataTable 지정 후 초기화 */
    void Initialize(UDataTable* InWaveTable, UDataTable* InPatternTable);

    /** 매 프레임 호출 (StageManager가 Tick에서 위임) */
    void TickWave(float DeltaSeconds);

    /** 시간·인덱스 초기화 */
    void ResetWaves();

private:
    /** 누적 시간 */
    float ElapsedTime = 0.f;

    /** 데이터 테이블 참조 (Wave/Pattern) */
    UPROPERTY()
    TObjectPtr<UDataTable> WaveTable = nullptr;

    UPROPERTY()
    TObjectPtr<UDataTable> PatternTable = nullptr;

    /** Wave 테이블 Row 이름 정렬 캐시 */
    UPROPERTY()
    TArray<FName> WaveRowNames;

    /** 현재 진행 중인 웨이브 실행 목록 */
    UPROPERTY()
    TArray<FWaveRuntime> ActiveWaves;
    
private:
    /** 특정 웨이브의 실행 가능 여부 판단 및 실행 */
    void TryExecuteWave(FWaveRuntime& Wave, float DeltaSeconds);
    /** 하나의 웨이브 실행 */
    void ExecuteWave(const struct FPPStageWave& InWaveData);
    /** 스폰 위치 계산 */
    FVector CalcSpawnPos(const struct FPPSpawnPattern& Pattern, int32 IterIdx) const;
    /** 적 소환 (실제 Spawn 처리) */
    void SpawnEnemy(TSubclassOf<class APPEnemyCharacterBase> EnemyClass, const FVector& SpawnLocation, bool bIsElite);

private:
    // 화면 밖 소환을 위한 사전 계산값들
    FVector CachedScreenCenter = FVector::ZeroVector;

    // 0: Right, 1: Left, 2: Forward, 3: Backward (카메라 기준)
    TArray<FVector> CachedSpawnDirections;

    UPROPERTY()
    FVector CachedCameraForward = FVector::ForwardVector;

    UPROPERTY()
    float CachedScreenRadius = 2000.f; // 화면 밖 최소 거리 (조정 가능)

    UPROPERTY()
    FVector2D CachedOffscreenMin;

    // 오프스크린 소환을 위한 카메라 사각 영역 (초기화 시 1회만 계산)
    FVector2D CameraWorldMin;
    FVector2D CameraWorldMax;
    void CacheCameraWorldBounds(); // 화면 좌표 1회 캐싱

		
};
