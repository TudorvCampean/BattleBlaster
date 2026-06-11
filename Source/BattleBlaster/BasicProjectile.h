// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "BasicProjectile.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasicProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ABasicProjectile();

	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

private:
	static constexpr float MinProjectileDamage = 0.0f;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Mechanic")
	int32 ProjectileMaxBounce = 5;

	int32 ProjectileCurrentBounce = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Mechanic")
	float DamageDecreasePerBounce = 5.0f;
	
public:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


};
