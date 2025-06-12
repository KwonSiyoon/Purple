// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/PPWaveSpawnManager.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"      // AEnemyBase 파생 가정
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Math/UnrealMathUtility.h"

#include "GameData/PPStageWave.h"      // FStageWaveRow
#include "GameData/PPSpawnPattern.h"   // FSpawnPatternRow
#include "Enemy/PPEnemyCharacterBase.h"

// Sets default values for this component's properties
UPPWaveSpawnManager::UPPWaveSpawnManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPPWaveSpawnManager::Initialize(UDataTable* InWaveTable, UDataTable* InPatternTable)
{
	WaveTable = InWaveTable;
	PatternTable = InPatternTable;

	WaveRowNames = WaveTable ? WaveTable->GetRowNames() : TArray<FName>();
	WaveRowNames.Sort([](const FName& A, const FName& B) { return A.LexicalLess(B); });

	ResetWaves();

	CacheCameraWorldBounds();

}

void UPPWaveSpawnManager::TickWave(float DeltaSeconds)
{
	if (!WaveTable || !PatternTable) return;

	ElapsedTime += DeltaSeconds;

	for (FWaveRuntime& Wave : ActiveWaves)
	{
		TryExecuteWave(Wave, DeltaSeconds);
	}
}

void UPPWaveSpawnManager::TryExecuteWave(FWaveRuntime& Wave, float DeltaSeconds)
{
	if (Wave.bFinished || !Wave.WaveData) return;

	const float StartTime = Wave.WaveData->StartTime;
	const float EndTime = Wave.WaveData->EndTime;

	if (ElapsedTime >= StartTime && ElapsedTime <= EndTime)
	{
		Wave.Accumulator += DeltaSeconds;
		if (Wave.Accumulator >= Wave.WaveData->Interval)
		{
			ExecuteWave(*Wave.WaveData);
			Wave.Accumulator = 0.f;

			if (!Wave.WaveData->bLoop)
			{
				Wave.bFinished = true;
			}
		}
	}
	else if (ElapsedTime > EndTime)
	{
		Wave.bFinished = true;
	}
}

void UPPWaveSpawnManager::ResetWaves()
{
	ElapsedTime = 0.f;
	ActiveWaves.Empty();

	for (const FName& RowName : WaveRowNames)
	{
		const FPPStageWave* Row = WaveTable->FindRow<FPPStageWave>(RowName, TEXT("ResetWaves"));
		if (Row)
		{
			ActiveWaves.Add({ RowName, Row, false, 0.f });
		}
	}
}

void UPPWaveSpawnManager::ExecuteWave(const FPPStageWave& InWaveData)
{
	const FPPSpawnPattern* Pattern = PatternTable->FindRow<FPPSpawnPattern>(InWaveData.PatternID, TEXT("PatternExec"));
	if (!Pattern) return;

	const TSubclassOf<APPEnemyCharacterBase> EnemyClass = InWaveData.EnemyBlueprint.LoadSynchronous();

	for (int32 i = 0; i < Pattern->Count; ++i)
	{
		const FVector SpawnLoc = CalcSpawnPos(*Pattern, i);
		SpawnEnemy(EnemyClass, SpawnLoc, InWaveData.bIsElite);
	}
}

FVector UPPWaveSpawnManager::CalcSpawnPos(const FPPSpawnPattern& Pattern, int32 IterIdx) const
{
	const APawn* Player = GetWorld()->GetFirstPlayerController()
		? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr;

	const FVector Center = Player ? Player->GetActorLocation() : FVector::ZeroVector;

	switch (Pattern.PatternType)
	{
	case ESpawnPatternType::Circle:
	{
		FRandomStream Stream(FDateTime::Now().GetTicks() + IterIdx);
		const float Angle = Stream.FRandRange(0.f, 360.f);
		const FVector Dir = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)),
			FMath::Sin(FMath::DegreesToRadians(Angle)), 0.f);
		UE_LOG(LogTemp, Log, TEXT("WaveManager Circle 패턴."));
		return Center + Dir * Pattern.Radius;
	}
	case ESpawnPatternType::RearAttack:
	{
		const FVector Backward = Player ? -Player->GetActorForwardVector() : FVector::ForwardVector;
		UE_LOG(LogTemp, Log, TEXT("WaveManager RearAttack 패턴."));
		return Center + Backward * Pattern.Radius;
	}
	case ESpawnPatternType::FixedZone:
	{
		if (Pattern.FixedPoints.IsValidIndex(IterIdx))
		{
			UE_LOG(LogTemp, Log, TEXT("WaveManager FixedZone 패턴."));
			return Pattern.FixedPoints[IterIdx] + Center;
		}
		break;
	}
	case ESpawnPatternType::OffscreenRandom:
	{
		UE_LOG(LogTemp, Log, TEXT("WaveManager OffscreenRandom 패턴."));

		// 플레이어 기준 방향 설정
		FRandomStream Stream(FDateTime::Now().GetTicks() + IterIdx);

		const float MinOffsetX = CachedOffscreenMin.X;
		const float MinOffsetY = CachedOffscreenMin.Y;
		const float Extra = 10.f;

		const float RandX = Stream.FRandRange(0.f, 1.f) < 0.5f
			? Stream.FRandRange(-MinOffsetX - Extra, -MinOffsetX - Extra * 2)
			: Stream.FRandRange(MinOffsetX + Extra, MinOffsetX + Extra * 2);

		const float RandY = Stream.FRandRange(0.f, 1.f) < 0.5f
			? Stream.FRandRange(-MinOffsetY - Extra, -MinOffsetY - Extra * 2)
			: Stream.FRandRange(MinOffsetY + Extra, MinOffsetY + Extra * 2);

		return Center + FVector(RandX, RandY, 0.f);
	}
	default:
	{
		return Center + FVector(1000.f, 0.f, 0.f); // fallback
	}
	}
	return Center + FVector(1000.f, 0.f, 0.f);
}

void UPPWaveSpawnManager::SpawnEnemy(TSubclassOf<APPEnemyCharacterBase> EnemyClass, const FVector& SpawnLocation, bool bIsElite)
{
	UE_LOG(LogTemp, Log, TEXT("WaveManager SpawnEnemy 시도."));
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnEnemy - EnemyClass is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("WaveManager EnemyClass 가져옴."));


	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto* Spawned = GetWorld()->SpawnActor<APPEnemyCharacterBase>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, Params);
	if (Spawned && bIsElite)
	{
		UE_LOG(LogTemp, Log, TEXT("WaveManager Elite 가져옴."));
	}
}

void UPPWaveSpawnManager::CacheCameraWorldBounds()
{
	if (const APlayerCameraManager* CamMgr = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		const FVector CamLoc = CamMgr->GetCameraLocation();
		TArray<FVector> Corners;

		const float Width = 1600.f;
		const float Height = 900.f;

		Corners.Add(CamLoc + FVector(Width, Height, 0.f));
		Corners.Add(CamLoc + FVector(-Width, Height, 0.f));
		Corners.Add(CamLoc + FVector(Width, -Height, 0.f));
		Corners.Add(CamLoc + FVector(-Width, -Height, 0.f));

		const APawn* Player = GetWorld()->GetFirstPlayerController()
			? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr;

		const FVector PlayerLoc = Player ? Player->GetActorLocation() : FVector::ZeroVector;

		float MaxX = 0.f;
		float MaxY = 0.f;

		for (const FVector& Pt : Corners)
		{
			const FVector2D Diff = FVector2D(Pt - PlayerLoc);
			MaxX = FMath::Max(MaxX, FMath::Abs(Diff.X));
			MaxY = FMath::Max(MaxY, FMath::Abs(Diff.Y));
		}

		CachedOffscreenMin = FVector2D(MaxX, MaxY);
	}
}




