// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnManager.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/ComputerCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerTarget = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	StartSpawning();
}

void ASpawnManager::StartSpawning()
{
	if (!TimelineData || TimelineData->TimelinePhases.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnManager: Missing TimelineDataAsset!"));
		return;
	}	
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASpawnManager::UpdateGameTime, 1.0f, true);
	AttemptEnemySpawn();
}

void ASpawnManager::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
}

void ASpawnManager::UpdateGameTime()
{
	SurvivalTime += 1.0f;
	
	const FSpawnPhase* CurrentPhase = GetCurrentPhase();
	if (CurrentPhase && !GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnManager::AttemptEnemySpawn, CurrentPhase->SpawnInterval, true);
	}
}

const FSpawnPhase* ASpawnManager::GetCurrentPhase() const
{
	if (!TimelineData) return nullptr;
	const FSpawnPhase* BestPhase = nullptr;
	
	for (const FSpawnPhase& Phase : TimelineData->TimelinePhases)
	{
		if (SurvivalTime >= Phase.StartTimeSeconds)
		{
			BestPhase = &Phase;
		}
	}

	return BestPhase;
}

void ASpawnManager::AttemptEnemySpawn()
{
	const FSpawnPhase* CurrentPhase = GetCurrentPhase();
	if (!CurrentPhase || !PlayerTarget) return;
	
	if (ActiveEnemyCount >= CurrentPhase->MaxActiveEnemiesCap)
	{
		return;
	}
	
	for (int32 i = 0; i < CurrentPhase->EnemiesPerSpawnTick; i++)
	{
		if (ActiveEnemyCount >= CurrentPhase->MaxActiveEnemiesCap) break;

		TSubclassOf<AComputerCharacter> SelectedClass = GetWeightedRandomEnemyClass(*CurrentPhase);
		if (!SelectedClass) continue;

		FVector SpawnPos = GetRandomRingSpawnPosition();
		FRotator SpawnRot = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AComputerCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AComputerCharacter>(SelectedClass, SpawnPos, SpawnRot, SpawnParams);
		if (SpawnedEnemy)
		{
			ActiveEnemyCount++;
		}
	}
}

TSubclassOf<AComputerCharacter> ASpawnManager::GetWeightedRandomEnemyClass(const FSpawnPhase& CurrentPhase) const
{
	if (CurrentPhase.EnemyPool.Num() == 0) return nullptr;

	float TotalWeight = 0.0f;
	for (const auto& Entry : CurrentPhase.EnemyPool)
	{
		TotalWeight += Entry.Weight;
	}

	float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
	float CumulativeWeight = 0.0f;

	for (const auto& Entry : CurrentPhase.EnemyPool)
	{
		CumulativeWeight += Entry.Weight;
		if (RandomValue <= CumulativeWeight)
		{
			return Entry.EnemyClass;
		}
	}

	return CurrentPhase.EnemyPool[0].EnemyClass;
}

FVector ASpawnManager::GetRandomRingSpawnPosition() const
{
	if (!PlayerTarget) return FVector::ZeroVector;

	float Angle = FMath::FRandRange(0.0f, TWO_PI);
	float Radius = FMath::FRandRange(MinSpawnRadius, MaxSpawnRadius);

	FVector Offset(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
	return PlayerTarget->GetActorLocation() + Offset;
}

void ASpawnManager::OnEnemyKilled()
{
	ActiveEnemyCount = FMath::Max(0, ActiveEnemyCount - 1);
}

