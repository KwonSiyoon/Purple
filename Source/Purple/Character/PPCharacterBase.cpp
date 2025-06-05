// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "PPCharacterControlData.h"
#include "EnhancedInputSubsystems.h"
#include "Enemy/PPEnemyCharacterBase.h"
#include "Skill/PPProjectileSkill.h"
#include "Projectile/PPProjectileBase.h"
#include "Projectile/PPProjectileData.h"
#include "PPCollision.h"

// Sets default values
APPCharacterBase::APPCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트 설정.
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PPPLAYER);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// Mesh 설정. 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));
	if (CharacterMesh.Object)
	{
		// Mesh에 접근해서 설정하는 방법.
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	// Animation Blueprint 설정.
	// Class니까 _C 추가.
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (CharacterAnim.Class)
	{
		GetMesh()->SetAnimClass(CharacterAnim.Class);
	}
	
	static ConstructorHelpers::FObjectFinder<UPPCharacterControlData> QuarterDataRef(TEXT("/Script/Purple.PPCharacterControlData'/Game/Purple/CharacterControl/PPC_Quarter.PPC_Quarter'"));
	if (QuarterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quarter, QuarterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UPPCharacterControlData> HorseViewDataRef(TEXT("/Script/Purple.PPCharacterControlData'/Game/Purple/CharacterControl/PPC_HorseView.PPC_HorseView'"));
	if (HorseViewDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Horse, HorseViewDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuarterMoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Purple/Input/IA_Move.IA_Move'"));
	if (QuarterMoveActionRef.Object)
	{
		QuarterMoveAction = QuarterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseSkillSlotAction_OneRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Purple/Input/IA_Skill1.IA_Skill1'"));
	if (UseSkillSlotAction_OneRef.Object)
	{
		UseSkillSlotAction_One = UseSkillSlotAction_OneRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseSkillSlotAction_TwoRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Purple/Input/IA_Skill2.IA_Skill2'"));
	if (UseSkillSlotAction_TwoRef.Object)
	{
		UseSkillSlotAction_Two = UseSkillSlotAction_TwoRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseSkillSlotAction_ThreeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Purple/Input/IA_Skill3.IA_Skill3'"));
	if (UseSkillSlotAction_ThreeRef.Object)
	{
		UseSkillSlotAction_Three = UseSkillSlotAction_ThreeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UseSkillSlotAction_FourRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Purple/Input/IA_Skill4.IA_Skill4'"));
	if (UseSkillSlotAction_FourRef.Object)
	{
		UseSkillSlotAction_Four = UseSkillSlotAction_FourRef.Object;
	}




	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 700.f;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f; 
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeRotation(FRotator(0.0f, 45.0f, 0.0f));


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CurrentCharacterControlType = ECharacterControlType::Quarter;
	ProjectileSkillClass = UPPProjectileSkill::StaticClass();
	OwnedSkills.Add(EPlayerSkillType::Fireball, UPPProjectileSkill::StaticClass());
	EquippedSkills.SetNum(4);
	EquippedSkills[0] = EPlayerSkillType::Fireball;
	EquippedSkills[1] = EPlayerSkillType::Fireball;
	EquippedSkills[2] = EPlayerSkillType::Fireball;
	EquippedSkills[3] = EPlayerSkillType::Fireball;

}

float APPCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

// Called when the game starts or when spawned
void APPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetCharacterControl(CurrentCharacterControlType);

	//GetWorld()->SpawnActor<APPEnemyCharacterBase>(APPEnemyCharacterBase::StaticClass(), GetActorLocation() + FVector::ForwardVector * 100.0f, FRotator::ZeroRotator);
}

void APPCharacterBase::QuarterMove(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();
	//float InputSizeSquared = Movement.SizeSquared();
	float MovementvectorSize = 1.0f;
	float MovementVectorSizeSquared = Movement.SizeSquared();

	// 두 방향으로 입력이 들어오면, 이동 방향은 정규화해 크기를 1로 만들고
	// 입력 스케일을 1로 강제 설정.
	if (Movement.SquaredLength() > 1.0f)
	{
		Movement.Normalize();
		MovementvectorSize = 1.0f;
	}
	// 입력이 1 이하이면, 해당 입력을 스케일로 사용하기 위해 값 계산.
	else
	{
		MovementvectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(Movement.X, Movement.Y, 0.0f);

	// 캐릭터가 이동하는 방향에 맞게 컨트롤러 회전 설정.
	Controller->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

	// 입력에 따른 방향으로 이동하도록 입력 전달.
	AddMovementInput(MoveDirection, MovementvectorSize);
}

void APPCharacterBase::UseActiveSkill(EPlayerSkillType SkillType)
{
	if (OwnedSkills.Contains(SkillType))
	{
		UPPProjectileSkill* SkillInstance = NewObject<UPPProjectileSkill>(this, OwnedSkills[SkillType]);
		SkillInstance->Initialize(this);
		SkillInstance->SetProjectileClass(APPProjectileBase::StaticClass());
		if (SkillInstance)
		{
			SkillInstance->UseSkill();
		}
	}
}

void APPCharacterBase::OnUseSkillSlot_One(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	if (BoolValue)
	{
		UE_LOG(LogTemp, Log, TEXT("SKill One"));
		if (EquippedSkills.IsValidIndex(0))
		{
			UseActiveSkill(EquippedSkills[0]);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SkillSlot One is Nothing."));
		}
	}

}

void APPCharacterBase::OnUseSkillSlot_Two(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	if (BoolValue)
	{
		UE_LOG(LogTemp, Log, TEXT("SKill Two"));
		if (EquippedSkills.IsValidIndex(1))
		{
			UseActiveSkill(EquippedSkills[1]);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SkillSlot Two is Nothing."));
		}
	}
}

void APPCharacterBase::OnUseSkillSlot_Three(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	if (BoolValue)
	{
		UE_LOG(LogTemp, Log, TEXT("SKill Three"));
		if (EquippedSkills.IsValidIndex(2))
		{
			UseActiveSkill(EquippedSkills[2]);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SkillSlot Three is Nothing."));
		}
	}
}

void APPCharacterBase::OnUseSkillSlot_Four(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	if (BoolValue)
	{
		UE_LOG(LogTemp, Log, TEXT("SKill Four"));
		if (EquippedSkills.IsValidIndex(3))
		{
			UseActiveSkill(EquippedSkills[3]);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SkillSlot Four is Nothing."));
		}
	}
}


// Called to bind functionality to input
void APPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(QuarterMoveAction, ETriggerEvent::Triggered, this, &APPCharacterBase::QuarterMove);
	EnhancedInputComponent->BindAction(UseSkillSlotAction_One, ETriggerEvent::Triggered, this, &APPCharacterBase::OnUseSkillSlot_One);
	EnhancedInputComponent->BindAction(UseSkillSlotAction_Two, ETriggerEvent::Triggered, this, &APPCharacterBase::OnUseSkillSlot_Two);
	EnhancedInputComponent->BindAction(UseSkillSlotAction_Three, ETriggerEvent::Triggered, this, &APPCharacterBase::OnUseSkillSlot_Three);
	EnhancedInputComponent->BindAction(UseSkillSlotAction_Four, ETriggerEvent::Triggered, this, &APPCharacterBase::OnUseSkillSlot_Four);
	

}

void APPCharacterBase::SetCharacterControlData(const UPPCharacterControlData* InCharacterControlData)
{
	// Pawn.
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement.
	GetCharacterMovement()->bOrientRotationToMovement = InCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = InCharacterControlData->RotationRate;

	// SpringArm 관련 설정.
	SpringArm->TargetArmLength = InCharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(InCharacterControlData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = InCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = InCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = InCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = InCharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = InCharacterControlData->bDoCollisionTest;
}

void APPCharacterBase::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	// 변경할 컨트롤 타입에 대응하는 데이터 애셋 로드(TMap으로부터).
	UPPCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	// 데이터 애셋을 사용해 관련 값 설정.
	SetCharacterControlData(NewCharacterControl);

	// Add InputMapping Context to Enhanced Input System.
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(
			NewCharacterControl->InputMappingContext,
			0
		);
	}

}

