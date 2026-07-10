// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ComputerCharacter.h"
#include "TowerEnemyTest.generated.h"

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
	virtual void Fire() override; 

	UPROPERTY(VisibleAnywhere, Category = "Components") UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components") UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components") UStaticMeshComponent* TurretBarrelMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components") USceneComponent* ProjectileSpawnPoint;
private:
	void RotateTurretHorizontal(FVector LookAtTarget);
	void AimBarrelVertical(FVector LookAtTarget);
};
