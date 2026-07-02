// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterModifier.generated.h"

class ABaseCharacter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BATTLEBLASTER_API UCharacterModifier : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Modifier Hooks")
	void OnApply(ABaseCharacter* TargetCharacter);
	virtual void OnApply_Implementation(ABaseCharacter* TargetCharacter){}


	UFUNCTION(BlueprintNativeEvent, Category = "Modifier Hooks")
	void OnTick(ABaseCharacter* TargetCharacter, float DeltaTime);
	virtual void OnTick_Implementation(ABaseCharacter* TargetCharacter, float DeltaTime) {}

	UFUNCTION(BlueprintNativeEvent, Category = "Modifier Hooks")
	void OnRemove(ABaseCharacter* TargetCharacter);
	virtual void OnRemove_Implementation(ABaseCharacter* TargetCharacter){}

	UFUNCTION(BlueprintNativeEvent, Category = "Modifier Hooks")
	float ModifyIncomingDamage(float Damage);
	virtual float ModifyIncomingDamage_Implementation(float Damage) { return Damage; }

	
};
