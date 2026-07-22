// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" 
#include "GameEntities/BattleBlasterGameMode.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLEBLASTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const { return Health / MaxHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void CheckHealth(float Damage, AActor* DamagedActor);

private:
	UPROPERTY()
	ABattleBlasterGameMode* BattleBlasterGameMode;
	bool bIsDead = false;
};
