#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCharacterModifier;

UCLASS(Blueprintable, BlueprintType)
class BATTLEBLASTER_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction() override;
	virtual void Fire() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* TurretBarrelMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Control|Input") UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category = "Control|Input") UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Control|Input") UInputAction* TurnAction;
	UPROPERTY(EditAnywhere, Category = "Control|Input") UInputAction* FireAction;
	UPROPERTY(EditAnywhere, Category = "Control|Input") UInputAction* JumpAction;
	UPROPERTY(VisibleAnywhere) USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere) UCameraComponent* CameraComp;

	
protected:	
	virtual void BeginPlay() override;

private:
	void MoveInput(const FInputActionValue& Value);
	void TurnInput(const FInputActionValue& Value);
	void HandleJump();
	void RotateTurretHorizontal(FVector LookAtTarget);
	void AimBarrelVertical(FVector LookAtTarget);
	

	APlayerController* PlayerController;
};