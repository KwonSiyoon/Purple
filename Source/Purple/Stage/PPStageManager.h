// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPStageManager.generated.h"

UCLASS()
class PURPLE_API APPStageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPStageManager();

    UFUNCTION(BlueprintCallable)
    void StartStage();

    /** 웨이브/패턴 테이블 (블루프린트에서 할당) */
    UPROPERTY(EditAnywhere, Category = "Wave")
    TObjectPtr<UDataTable> StageWaveTable;

    UPROPERTY(EditAnywhere, Category = "Wave")
    TObjectPtr<UDataTable> SpawnPatternTable;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    /** SpawnManager는 Subobject이므로 UPROPERTY로 관리 */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UPPWaveSpawnManager> WaveManager;

};
