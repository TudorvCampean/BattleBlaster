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
	virtual void HandleDestruction() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float FireRange = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float FireRate = 1.5f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	APlayerCharacter* PlayerTarget;
	FTimerHandle AttackTimerHandle;
	void CheckAttackCondition();
	bool IsPlayerInAttackRange();
};