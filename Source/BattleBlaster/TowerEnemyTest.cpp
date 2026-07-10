// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerEnemyTest.h"
#include "Characters/PlayerCharacter.h"
#include "Projectiles/Projectile.h"

ATowerEnemyTest::ATowerEnemyTest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());
	BaseMesh->SetCanEverAffectNavigation(false);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	TurretMesh->SetCanEverAffectNavigation(false);

	TurretBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBarrelMesh"));
	TurretBarrelMesh->SetupAttachment(TurretMesh);
	TurretBarrelMesh->SetCanEverAffectNavigation(false);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretBarrelMesh);
}

void ATowerEnemyTest::Tick(float DeltaTime)
{	Super::Tick(DeltaTime);

	
	if (PlayerTarget && IsPlayerInAttackRange())
	{
		RotateTurretHorizontal(PlayerTarget->GetActorLocation());
		AimBarrelVertical(PlayerTarget->GetActorLocation());
	}
}

void ATowerEnemyTest::Fire()
{
	if (!ProjectileClass || !ProjectileSpawnPoint) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation(),
		SpawnParams
	);
}

void ATowerEnemyTest::RotateTurretHorizontal(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void ATowerEnemyTest::AimBarrelVertical(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretBarrelMesh->GetComponentLocation();
	FVector LocalDirection = TurretMesh->GetComponentTransform().InverseTransformVector(VectorToTarget);
	float ClampedPitch = FMath::Clamp(LocalDirection.Rotation().Pitch, -10.0f, 90.0f);
	TurretBarrelMesh->SetRelativeRotation(FRotator(ClampedPitch, 0.0f, 0.0f));
}
