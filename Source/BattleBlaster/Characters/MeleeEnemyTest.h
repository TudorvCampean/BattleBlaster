// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ComputerCharacter.h"
#include "MeleeEnemyTest.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API AMeleeEnemyTest : public AComputerCharacter
{
	GENERATED_BODY()

public:
	AMeleeEnemyTest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackDamage = 15.0f;

protected:	
	virtual void CheckAttackCondition() override;

};
