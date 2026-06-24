// Fill out your copyright notice in the Description page of Project Settings.


#include "Modifiers/ProjectileModifiers/BouncingModifier.h"
#include "Projectiles/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "BouncingModifier.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UBouncingModifier::OnProjectileSpawn_Implementation(AProjectile* Projectile)
{
	if (Projectile && Projectile->ProjectileMovementComponent)	{
		
		Projectile->ProjectileMovementComponent->bShouldBounce = true;		
		Projectile->ProjectileMovementComponent->Bounciness = 1.0f;
		Projectile->ProjectileMovementComponent->Friction = 0.0f;		
	}
}

bool UBouncingModifier::OnProjectileHit_Implementation(AProjectile* Projectile, const FHitResult& Hit, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(APawn::StaticClass())) {
		return false;
	}

	if (CurrentBounces < MaxBounces) {
		CurrentBounces++;		
		Projectile->SetLifeSpan(15.0f);
		
		return true;
	}

	if (Projectile && Projectile->ProjectileMovementComponent)
	{
		Projectile->ProjectileMovementComponent->bShouldBounce = false;
	}
	return false;
}
