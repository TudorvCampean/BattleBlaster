#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ComputerCharacter.generated.h"

class APlayerCharacter;

UCLASS(Blueprintable)
class BATTLEBLASTER_API AComputerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AComputerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	bool bShouldChasePlayer = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float AttackStoppingDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float AttackRange = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float AttackRate = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Rewards")
	float ExperienceReward = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void ActivateEnemy();
	virtual void HandleDestruction() override;

protected:
	virtual void BeginPlay() override;
	virtual void UpdatePathfinding();
	virtual void CheckAttackCondition();
	virtual bool IsPlayerInAttackRange();

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	APlayerCharacter* PlayerTarget;

private:
	FTimerHandle MovementTimerHandle;
	FTimerHandle AttackTimerHandle;
};