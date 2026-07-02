// Fill out your copyright notice in the Description page of Project Settings.


#include "Modifiers/CharacterModifiers/ShieldModifier.h"
#include "Characters/HealthComponent.h"
#include "Characters/BaseCharacter.h"
#include "Components/StaticMeshComponent.h"

UShieldModifier::UShieldModifier()
{
	CurrentShieldHP = 25.0f;
	TimeSinceLastDamage = 0.0f;
	VisualShieldComponent = nullptr;
}

void UShieldModifier::OnApply_Implementation(ABaseCharacter* TargetCharacter)
{
	if (!TargetCharacter)
		return;

	CurrentShieldHP = MaxShieldHP;
	TimeSinceLastDamage = RegenDelay;

	VisualShieldComponent = NewObject <UStaticMeshComponent>(TargetCharacter, TEXT("DynamicShieldMesh"));
	if (VisualShieldComponent) {
		VisualShieldComponent->RegisterComponent();
		VisualShieldComponent->SetStaticMesh(ShieldMeshAsset);

		VisualShieldComponent->SetupAttachment(TargetCharacter->GetRootComponent());
		VisualShieldComponent->SetCollisionProfileName(TEXT("NoCollision"));
		VisualShieldComponent->SetVisibility(true);

		VisualShieldComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

void UShieldModifier::OnTick_Implementation(ABaseCharacter* TargetCharacter, float DeltaTime)
{
	if (CurrentShieldHP < MaxShieldHP) {
		TimeSinceLastDamage += DeltaTime;
		if (TimeSinceLastDamage >= RegenDelay) {
			CurrentShieldHP += RegenRate * DeltaTime;
			CurrentShieldHP = FMath::Min(CurrentShieldHP, MaxShieldHP);

			if (CurrentShieldHP > 0.0f && VisualShieldComponent && !VisualShieldComponent->IsVisible())
			{
				VisualShieldComponent->SetVisibility(true);
			}
		}
	}
}

void UShieldModifier::OnRemove_Implementation(ABaseCharacter* TargetCharacter)
{
	if (VisualShieldComponent) 
		VisualShieldComponent->DestroyComponent();
}

float UShieldModifier::ModifyIncomingDamage_Implementation(float Damage)
{
	TimeSinceLastDamage = 0.0f;

	if (CurrentShieldHP <= 0.0f || Damage <= 0.0f)
		return Damage;

	float DamageAbsorbed = FMath::Min(Damage, CurrentShieldHP);
	CurrentShieldHP -= DamageAbsorbed;

	if (CurrentShieldHP <= 0.0f && VisualShieldComponent) {
		VisualShieldComponent->SetVisibility(false);
	}

	return Damage - DamageAbsorbed;
}

