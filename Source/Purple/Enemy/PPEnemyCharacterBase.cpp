// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PPEnemyCharacterBase.h"
#include "AI/PPAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PPCollision.h"

// Sets default values
APPEnemyCharacterBase::APPEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러의 회전을 받아서 설정하는 모드를 모두 해제.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// 컴포넌트 설정.
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PPENEMY);

	// 무브먼트 설정.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// 메시의 콜리전은 NoCollision 설정(주로 랙돌에 사용됨).
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//GetMesh()->SetHiddenInGame(true);

	AIControllerClass = APPAIController::StaticClass();

	/** Pawn is automatically possessed by an AI Controller whenever it is created. */
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

float APPEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

// Called when the game starts or when spawned
void APPEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	UE_LOG(LogTemp, Log, TEXT("%s"), *Player->GetName());

	APPAIController* AIController = Cast<APPAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("Target", Player);
		UE_LOG(LogTemp, Log, TEXT("%s"), *AIController->GetName());
	}

}

void APPEnemyCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void APPEnemyCharacterBase::AttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 캐릭터 무브먼트 컴포넌트 모드 복구.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// 전달 받은 델리게이트 실행.
	OnAttackFinished.ExecuteIfBound();
}

// Called every frame
void APPEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APPEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APPEnemyCharacterBase::GetAIAttackRange()
{
	return 100.0f;
}

float APPEnemyCharacterBase::GetAITurnSpeed()
{
	return 2.0f;
}

void APPEnemyCharacterBase::AttackByAI()
{
	// 이동 비활성화.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 몽타주 재생.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		const float AttackSpeedRate = 1.0f;
		//const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
		AnimInstance->Montage_Play(AttackMontage, AttackSpeedRate);

		// 몽타주 재생이 시작되면, 재생이 종료될 때 호출되는 델리게이트에 등록.
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APPEnemyCharacterBase::AttackActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	}
}

void APPEnemyCharacterBase::SetAIAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APPEnemyCharacterBase::AttackHitCheck()
{
	// 충돌 시작 지점 계산.
	// 캐릭터 몸통에서 약간 앞으로(캡슐의 반지름 만큼) 설정.
	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 공격 거리.
	const float AttackRange = GetAIAttackRange();
	//const float AttackRange = Stat->GetTotalStat().AttackRange;
	FVector End = Start + GetActorForwardVector() * AttackRange;

	// SCENE_QUERY_STAT: 언리얼에서 지원하는 분석 툴에 태그를 추가.
	// 두번째 인자: 복잡한 형태의 충돌체를 감지할 지 여부.
	// 세번째 인자: 무시할 액터 목록.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// 트레이스에 사용할 구체의 반지름.
	const float AttackRadius = 50.0f;
	//const float AttackRadius = Stat->GetAttackRadius();

	// 트레이스를 활용해 충돌 검사.
	FHitResult OutHitResult;
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_PPENEMY, FCollisionShape::MakeSphere(AttackRadius), Params);

	// 충돌 감지된 경우의 처리.
	if (HitDetected)
	{
		// 데미지 양.
		const float AttackDamage = 10.0f;
		//const float AttackDamage = Stat->GetTotalStat().Attack;

		// 데미지 이벤트.
		FDamageEvent DamageEvent;

		// 데미지 전달.
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	// 충돌 디버그(시각적으로 확인할 수 있도록).
#if ENABLE_DRAW_DEBUG

	// 캡슐의 중심 위치.
	//FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	FVector CapsuleOrigin = (End + Start) * 0.5f;

	// 캡슐 높이 절반 값.
	float CapsuleHalfHeight = AttackRange * 0.5f;

	// 표시할 색상(안 맞았으면 빨강, 맞았으면 초록).
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	//FMath::RandRange
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif

}

