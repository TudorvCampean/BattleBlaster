// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ComputerCharacter.h"
#include "TowerEnemyTest.generated.h"

class AProjectile;

/**
 * First enemy created as a test for the AI movement and firing system
 */
UCLASS(Blueprintable)
class BATTLEBLASTER_API ATowerEnemyTest : public AComputerCharacter
{
	GENERATED_BODY()
public:
	ATowerEnemyTest();
	virtual void Tick(float DeltaTime) override;
	virtual void Attack() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName MuzzleSocketName = TEXT("Muzzle");

};
