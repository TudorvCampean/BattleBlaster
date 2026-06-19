// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Projectiles/Projectile.h"
#include "BasePawn.generated.h"


UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();


	UPROPERTY(VisibleAnywhere, Category = "Pawn Components|PawnCore")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Components|PawnBase")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Components|PawnBody")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Components|PawnBarrel")
	UStaticMeshComponent* TurretBarrelMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Components|PawnProjectile")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "ControlProperties|Combat")
	float MaxPitchAngle = 90.0f;

	UPROPERTY(EditAnywhere, Category = "ControlProperties|Combat")
	float MinPitchAngle = -10.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* DestructionParticles;
	
	void RotateTurretHorizontal(FVector LookAtTarget);
	void AimBarrelVertical(FVector LookAtTarget);

	virtual void Fire();

	void HandleDestruction();


};
