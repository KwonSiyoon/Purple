// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PPAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

APPAIController::APPAIController()
{
	// 애셋 로드.
	// 블랙 보드 애셋.
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT(""));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT(""));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void APPAIController::RunAI()
{
	// 블랙보드 컴포넌트 포인터 가져오기.
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	// 사용할 블랙보드 지정.
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 시작할 때 NPC의 위치를 블랙보드의 HomePos에 저장.
		//Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 행동 트리 실행.
		bool RunResult = RunBehaviorTree(BTAsset);

		// 실행에 실패하면 확인.
		ensure(RunResult);
	}
}

void APPAIController::StopAI()
{
	// 실행 중인 BehaviorTreeComponent 가져오기.
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	// BT 중지.
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void APPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
