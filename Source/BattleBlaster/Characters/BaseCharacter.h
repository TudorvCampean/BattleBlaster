#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UNiagaraSystem;
class USoundBase;
class UCharacterModifier;

UCLASS(Abstract)
class BATTLEBLASTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UNiagaraSystem* DestructionParticles;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers")
	TArray<TSubclassOf<UCharacterModifier>> InitialModifiers;

	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void AddCharacterModifier(TSubclassOf<UCharacterModifier> ModifierClass);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
	virtual void Attack() {}
	virtual void HandleDestruction();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<UCharacterModifier*> ActiveModifiers;
};