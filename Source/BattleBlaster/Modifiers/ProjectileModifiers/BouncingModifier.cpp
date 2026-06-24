// Fill out your copyright notice in the Description page of Project Settings.


#include "Modifiers/ProjectileModifiers/BouncingModifier.h"
#include "Projectiles/Projectile.h"
#include "BouncingModifier.h"
#include "GameFramework/ProjectileMovementComponent.h"

bool UBouncingModifier::OnProjectileHit_Implementation(AProjectile* Projectile, const FHitResult& Hit, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(APawn::StaticClass())) {
		return false;
	}

	if (CurrentBounces < MaxBounces) {
		CurrentBounces++;

		if (Projectile && Projectile->ProjectileMovementComponent) {
			FVector CurrentVelocity = Projectile->ProjectileMovementComponent->Velocity;
			FVector ReflectedVelocity = CurrentVelocity.MirrorByVector(Hit.ImpactNormal);

			Projectile->ProjectileMovementComponent->Velocity = ReflectedVelocity;
			Projectile->SetActorRotation(ReflectedVelocity.Rotation());

			return true;
		}
	}
	return false;
}
