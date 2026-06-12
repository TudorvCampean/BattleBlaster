// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABasicProjectile::ABasicProjectile()
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 1.0f; // keep all speed
		ProjectileMovementComponent->Friction = 0.0f;   // doesn't slow down on friction
	}
}

void ABasicProjectile::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABasicProjectile, ProjectileMaxBounce) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ABasicProjectile, DamageDecreasePerBounce))
	{		
		Damage = static_cast<float>(ProjectileMaxBounce) * DamageDecreasePerBounce;
	}
}

void ABasicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
		return;

	if (OtherActor && OtherActor != this) {
		if (OtherActor != MyOwner || ProjectileCurrentBounce > 0) {
			if (OtherActor->IsA(APawn::StaticClass())) {
				UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, UDamageType::StaticClass());

				DisplayEffects(Hit);
				Destroy();
				return;
			}
		}

		ProjectileCurrentBounce++;
		Damage -= DamageDecreasePerBounce;

		if (Damage <= MinProjectileDamage || ProjectileCurrentBounce >= ProjectileMaxBounce) {
			DisplayEffects(Hit);
			Destroy();
		}
	}
}

