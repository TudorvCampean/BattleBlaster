// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifiers/CharacterModifiers/CharacterModifier.h"
#include "ShieldModifier.generated.h"


class UStaticMeshComponent;
class UStaticMesh;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BATTLEBLASTER_API UShieldModifier : public UCharacterModifier
{
	GENERATED_BODY()
public:
	UShieldModifier();

	UPROPERTY(EditAnywhere, Category = "Shield|Stats")
	float MaxShieldHP = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Shield|Stats")
	float RegenDelay = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Shield|Stats")
	float RegenRate = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Shield|Visual")
	UStaticMesh* ShieldMeshAsset;

	virtual void OnApply_Implementation(ABaseCharacter* TargetCharacter) override;
	virtual void OnTick_Implementation(ABaseCharacter* TargetCharacter, float DeltaTime) override;
	virtual void OnRemove_Implementation(ABaseCharacter* TargetCharacter) override;

	virtual float ModifyIncomingDamage_Implementation(float Damage) override;

private:
	float CurrentShieldHP;
	float TimeSinceLastDamage;

	UPROPERTY()
	UStaticMeshComponent* VisualShieldComponent;
	
};
