// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PPAIController.generated.h"

/**
 * 
 */
UCLASS()
class PURPLE_API APPAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APPAIController();

	// AI 실행하는 함수.
	void RunAI();

	// AI 중지하는 함수.
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:	// Data Section.
	// Blackboard 애셋.
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// Behavior Tree 애셋.
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
