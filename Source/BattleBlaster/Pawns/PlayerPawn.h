// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "PlayerPawn.generated.h" // S-a schimbat reflexia generată

class UCameraComponent;
class UInputMappingContext;

/**
 * Clasa generică pentru Jucător (fostul Tank)
 */
UCLASS(Blueprintable, BlueprintType)
class BATTLEBLASTER_API APlayerPawn : public ABasePawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	APlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Control|Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Control|Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Control|Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Control|Input")
	UInputAction* FireAction;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "ControlProperties|Movement")
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "ControlProperties|Movement")
	float TurnRate = 45.0f; // Ajustat pentru o rotație fluidă generală

	APlayerController* PlayerController;
	bool IsAlive;

	bool bCanFire = true;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireCooldown = 0.5f; // Timp redus (stil Isaac) în loc de 5 secunde!

	FTimerHandle FireTimerHandle;
	virtual void Fire() override;

	void MoveInput(const FInputActionValue& Value);
	void TurnInput(const FInputActionValue& Value);

	void HandleDestruction();
	void SetPlayerEnabled(bool Enabled);

	void ResetFireCooldown();
};