// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&ATower::CheckFireCondition,
		FireRate,
		true
	);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InFireRange() && PlayerPawn) {
		RotateTurretHorizontal(PlayerPawn->GetActorLocation());
		AimBarrelVertical(PlayerPawn->GetActorLocation());
	}
}

void ATower::Fire()
{
	Super::Fire();
}

void ATower::CheckFireCondition()
{
	if (PlayerPawn && PlayerPawn->IsAlive && InFireRange()) {
		Fire();
	}
}

bool ATower::InFireRange()
{
	if (PlayerPawn) {
		float Distance = FVector::Dist(this->GetActorLocation(), PlayerPawn->GetActorLocation());
		if (Distance <= FireRange) {
			return true;
		}
	}
	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}
