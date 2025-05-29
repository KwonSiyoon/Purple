// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PPEnemyCharacterBase.h"
#include "AI/PPAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

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
	//GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

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

// Called when the game starts or when spawned
void APPEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPEnemyCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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

