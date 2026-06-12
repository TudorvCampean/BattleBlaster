// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

ALaserProjectile::ALaserProjectile()
{
	Damage = 50.0f;

	if (ProjectileMovementComponent) {
		ProjectileMovementComponent->InitialSpeed = 5000.0f;
		ProjectileMovementComponent->MaxSpeed = 5000.0f;

		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 1.0f;
		ProjectileMovementComponent->Friction = 0.0f;

		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	}
}

void ALaserProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
		return;

	if (OtherActor && OtherActor != this) {
		if (OtherActor != MyOwner || CurrentLaserBounces > 0) {
			if (OtherActor->IsA(APawn::StaticClass())) {
				UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, UDamageType::StaticClass());

				Destroy();
				return;
			}
		}		

		CurrentLaserBounces++;

		if (CurrentLaserBounces >= MaxLaserBounces) {
			Destroy();
		}
	}
}
