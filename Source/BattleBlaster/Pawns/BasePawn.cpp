// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	TurretBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBarrelMesh"));
	TurretBarrelMesh->SetupAttachment(TurretMesh);


	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretBarrelMesh);
}

void ABasePawn::RotateTurretHorizontal(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f );
	
	FRotator InterpolatedRotation = FMath::RInterpTo(
		TurretMesh->GetComponentRotation(),
		LookAtRotation,
		GetWorld()->GetDeltaSeconds(),
		10.0f);

	TurretMesh->SetWorldRotation(InterpolatedRotation);
}

void ABasePawn::AimBarrelVertical(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretBarrelMesh->GetComponentLocation();
	// transform global vector in local space so we can aim even if we are sitting on a slope
	FVector LocalDirection = TurretMesh->GetComponentTransform().InverseTransformVector(VectorToTarget);
	FRotator LocalLookAtRotation = LocalDirection.Rotation();

	float ClampedPitch = FMath::Clamp(LocalLookAtRotation.Pitch, MinPitchAngle,MaxPitchAngle);
	FRotator TargetRelativeRotation = FRotator(ClampedPitch, 0.0f, 0.0f);

	FRotator IntepolatedRotation = FMath::RInterpTo(
		TurretBarrelMesh->GetRelativeRotation(),
		TargetRelativeRotation,
		GetWorld()->GetDeltaSeconds(),
		10.0f
	);

	TurretBarrelMesh->SetRelativeRotation(IntepolatedRotation);

}

void ABasePawn::Fire()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	if (Projectile) {
		Projectile->SetOwner(this);
	}

}

void ABasePawn::HandleDestruction()
{
	if (DestructionParticles) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DestructionParticles,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}



