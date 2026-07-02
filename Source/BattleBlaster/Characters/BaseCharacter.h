#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class AProjectile;
class UNiagaraSystem;
class USoundBase;
class UCharacterModifier;

UCLASS(Blueprintable)
class BATTLEBLASTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers")
	TArray<TSubclassOf<UCharacterModifier>> InitialModifiers;

	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void AddCharacterModifier(TSubclassOf<UCharacterModifier> ModifierClass);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void RotateTurretHorizontal(FVector LookAtTarget);
	void AimBarrelVertical(FVector LookAtTarget);

	virtual void Fire();
	virtual void HandleDestruction();

protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	TArray<UCharacterModifier*> ActiveModifiers;
};