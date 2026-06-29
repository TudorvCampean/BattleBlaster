#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class AProjectile;
class UNiagaraSystem;
class USoundBase;

UCLASS(Blueprintable)
class BATTLEBLASTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, Category = "Components|Mesh")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components|Mesh")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components|Mesh")
	UStaticMeshComponent* TurretBarrelMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components|Mesh")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "Components|Health")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* DestructionParticles;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;

	void RotateTurretHorizontal(FVector LookAtTarget);
	void AimBarrelVertical(FVector LookAtTarget);

	virtual void Fire();
	virtual void HandleDestruction();
};