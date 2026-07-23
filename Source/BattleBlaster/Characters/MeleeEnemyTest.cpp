// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MeleeEnemyTest.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AMeleeEnemyTest::AMeleeEnemyTest()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = 380.0f;
	AttackRange = 150.0f;            
	AttackStoppingDistance = 100.0f;  
	AttackRate = 1.2f;
}

void AMeleeEnemyTest::CheckAttackCondition()
{
	Super::CheckAttackCondition();

	if (IsPlayerInAttackRange() && PlayerTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Melee Enemy hit player for %f damage!"), AttackDamage);

		UGameplayStatics::ApplyDamage(
			PlayerTarget,
			AttackDamage,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
}

