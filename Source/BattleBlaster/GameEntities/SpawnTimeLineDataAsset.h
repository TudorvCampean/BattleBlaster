// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Characters/ComputerCharacter.h"
#include "SpawnTimeLineDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FEnemySpawnWeight
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AComputerCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
	float Weight = 100.0f;
};

USTRUCT(BlueprintType)
struct FSpawnPhase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FString PhaseName = "Wave Phase";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	float StartTimeSeconds = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 EnemiesPerSpawnTick = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxActiveEnemiesCap = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<FEnemySpawnWeight> EnemyPool;
};

UCLASS(BlueprintType)
class BATTLEBLASTER_API USpawnTimeLineDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline Config")
	TArray<FSpawnPhase> TimelinePhases;
};
