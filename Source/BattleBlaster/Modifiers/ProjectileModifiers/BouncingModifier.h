// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifiers/ProjectileModifiers/ProjectileModifier.h"
#include "BouncingModifier.generated.h"

/**
 * Modifier that allows the projectile to bounce
 */
UCLASS()
class BATTLEBLASTER_API UBouncingModifier : public UProjectileModifier
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier Stats")
	int32 MaxBounces = 3;

protected:
	virtual void OnProjectileSpawn_Implementation(AProjectile* Projectile) override;

	virtual bool OnProjectileHit_Implementation(AProjectile* Projectile, const FHitResult& Hit, AActor* OtherActor) override;

private:
	int32 CurrentBounces = 0;
};
