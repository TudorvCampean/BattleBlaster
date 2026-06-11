// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "LaserProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ALaserProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ALaserProjectile();
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Mechanic")
	int32 MaxLaserBounces = 12;

	int32 CurrentLaserBounces = 0;

};
