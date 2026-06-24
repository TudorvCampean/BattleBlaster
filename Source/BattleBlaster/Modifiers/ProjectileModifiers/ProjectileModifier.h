// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProjectileModifier.generated.h"

class AProjectile;
/**
 * Base class for all weapon upgrades
 */
UCLASS(Blueprintable, BlueprintType)
class BATTLEBLASTER_API UProjectileModifier : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Modifier Hooks")
	void OnProjectileSpawn(AProjectile* Projectile);
	virtual void OnProjectileSpawn_Implementation(AProjectile* Projectile) {}

	UFUNCTION(BlueprintNativeEvent, Category = "Modifier Hooks")
	void OnProjectileTick(AProjectile* Projectile, float DeltaTime);
	virtual void OnProjectileTick_Implementation(AProjectile* Projectile, float DeltaTime) {}

	UFUNCTION(BlueprintNativeEvent, Category = "Modifier Hooks")
	bool OnProjectileHit(AProjectile* Projectile, const FHitResult& Hit, AActor* OtherActor);
	virtual bool OnProjectileHit_Implementation(AProjectile* Projectile, const FHitResult& Hit, AActor* OtherActor) { return false; }

};
