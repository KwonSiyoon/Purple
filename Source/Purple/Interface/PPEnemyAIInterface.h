// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPEnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

// 공격 종료 델리게이트.
DECLARE_DELEGATE(FAIEnemyAttackFinished);


/**
 * 
 */
class PURPLE_API IPPEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
	virtual void AttackByAI() = 0;

	virtual void SetAIAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished) = 0;

};
