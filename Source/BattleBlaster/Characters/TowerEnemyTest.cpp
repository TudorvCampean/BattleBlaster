// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerEnemyTest.h"
#include "Characters/PlayerCharacter.h"
#include "Projectiles/Projectile.h"

ATowerEnemyTest::ATowerEnemyTest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATowerEnemyTest::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	if (PlayerTarget && IsPlayerInAttackRange())
	{
		FVector Direction = PlayerTarget->GetActorLocation() - GetActorLocation();
		FRotator TargetRotation = FRotator(0.0f, Direction.Rotation().Yaw, 0.0f);

		
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f));
	}	
}

void ATowerEnemyTest::Attack()
{
	if (!ProjectileClass) return;


	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FRotator SpawnRotation = GetMesh()->GetSocketRotation(MuzzleSocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}