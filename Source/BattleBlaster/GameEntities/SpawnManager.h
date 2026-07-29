// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnTimeLineDataAsset.h"
#include "SpawnManager.generated.h"

class AComputerCharacter;
class APlayerCharacter;

UCLASS()
class BATTLEBLASTER_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	void StartSpawning();
	void StopSpawning();
	void OnEnemyKilled();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Setup")
	USpawnTimeLineDataAsset* TimelineData;

	UPROPERTY(EditAnywhere, Category = "Spawn Setup")
	float MinSpawnRadius = 1300.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn Setup")
	float MaxSpawnRadius = 1800.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	APlayerCharacter* PlayerTarget;
	int32 ActiveEnemyCount = 0;
	float SurvivalTime = 0.0f;

	FTimerHandle SpawnTimerHandle;
	FTimerHandle GameTimerHandle;

	void UpdateGameTime();
	void AttemptEnemySpawn();

	const FSpawnPhase* GetCurrentPhase() const;
	TSubclassOf<AComputerCharacter> GetWeightedRandomEnemyClass(const FSpawnPhase& CurrentPhase) const;
	FVector GetRandomRingSpawnPosition() const;
};
