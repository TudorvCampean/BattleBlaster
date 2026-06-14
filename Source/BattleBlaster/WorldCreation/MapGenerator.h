// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UCLASS()
class BATTLEBLASTER_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings|Dimensions")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings|Dimensions")
	int32 GridHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings|Dimensions")
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings|Logic")
	int32 MapSeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings|Logic")
	bool bUseRandomSeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings|Assets")
	TArray<TSubclassOf<AActor>> AvailableTileClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings|Assets")
	TSubclassOf<AActor> BoundaryWallClass;

protected:

	UPROPERTY()
	TArray<AActor*> SpawnedTiles;

	FVector SafePlayerSpawnLocation;

public:

	UFUNCTION(BlueprintCallable, Category = "Map Functions")
	void GenerateMap();

	UFUNCTION(BlueprintCallable, Category = "Map Data")
	FVector GetPlayerSpawnLocation() const;

protected:

	void ClearExistingMap();
};
