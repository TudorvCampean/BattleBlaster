// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values
UHealthComponent::UHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode) {
		BattleBlasterGameMode = Cast<ABattleBlasterGameMode>(GameMode);
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.0f) {
		Health -= Damage;
		if (Health <= 0.0f) {
			if (BattleBlasterGameMode) {
				BattleBlasterGameMode->ActorDied(DamagedActor);
			}
		}
	}
}

