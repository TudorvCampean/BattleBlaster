// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UHealthComponent::UHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	bIsDead = false;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode) {
		BattleBlasterGameMode = Cast<ABattleBlasterGameMode>(GameMode);
	}
}


void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	CheckHealth(Damage, DamagedActor);
}

void UHealthComponent::CheckHealth(float Damage, AActor*DamagedActor)
{
	if (bIsDead)
		return;

	if (Damage > 0.0f) {
		Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
		if (Health <= 0.0f) {
			bIsDead = true;

			if (BattleBlasterGameMode) {
				BattleBlasterGameMode->ActorDied(DamagedActor);
			}

		}
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (bIsDead || HealAmount <= 0.0f) return;

	Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
}

