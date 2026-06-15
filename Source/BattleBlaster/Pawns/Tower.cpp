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

	if (InFireRange()) {				
		RotateTurretHorizontal(Tank->GetActorLocation());
		AimBarrelVertical(Tank->GetActorLocation());
	}
}

void ATower::Fire()
{
	Super::Fire();
}

void ATower::CheckFireCondition()
{
	if (Tank && Tank->IsAlive && InFireRange()) {
		Fire();		
	}
}

bool ATower::InFireRange()
{
	if (Tank) {
		float Distance = FVector::Dist(this->GetActorLocation(), Tank->GetActorLocation());
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
	UE_LOG(LogTemp, Display, TEXT("Tower Handle Destruction"));
}
