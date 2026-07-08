// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldCreation/MapGenerator.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GridWidth = 5;
	GridHeight = 5;
	TileSize = 1000.0f;
	MapSeed = 12345;
	bUseRandomSeed = true;
}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateMap();
}

void AMapGenerator::GenerateMap()
{
	if (AvailableTileClasses.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("MapGenerator: AvailableTileClasses is empty! Please assign tiles in the Blueprint Editor!"));
		return;
	}

	ClearExistingMap();

	if (bUseRandomSeed) {
		MapSeed = FMath::RandRange(1, 999999);
		UE_LOG(LogTemp, Log, TEXT("MapGenerator: Generating layout using MapSeed: %d"), MapSeed);
	}

	FRandomStream Stream(MapSeed);

	FVector MapOrigin = GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	// Math: Calculate half a tile size to compensate for center-pivoted meshes
	const float HalfTileOffset = TileSize * 0.5f;

	SafePlayerSpawnLocation = FVector::ZeroVector;

	for (int32 X = 0; X < GridWidth; ++X) {
		for (int32 Y = 0; Y < GridHeight; ++Y) {
			// Math: Converting XOY coordinates to WorldCoordinates
			const float XOffset = static_cast<float>(X) * TileSize + HalfTileOffset;
			const float YOffset = static_cast<float>(Y) * TileSize + HalfTileOffset;

			// Math: Combine the origin anchor with our offset to get the final 3D World placement
			const FVector SpawnLocation = MapOrigin + FVector(XOffset, YOffset, 0.f);			

			bool bIsBorder = (X == 0 || X == GridWidth - 1 || Y == 0 || Y == GridHeight - 1);
			bool bIsPlayerSpawnTile = (X == 2 && Y == 2);

			TSubclassOf<AActor> SelectedTileClass = nullptr;
			FRotator TileRotation = FRotator::ZeroRotator;

			if (bIsBorder) {
				SelectedTileClass = BoundaryWallClass;

				
				if (X == 0 && Y > 0) {					
					TileRotation = FRotator(0.f, -90.f, 0.f);
				}
				else if (X == GridWidth - 1 && Y < GridHeight - 1) {					
					TileRotation = FRotator(0.f, 90.f, 0.f);
				}				
				else if (Y == GridHeight - 1) {					
					TileRotation = FRotator(0.f, 180.f, 0.f);
				}
			}
			else if (bIsPlayerSpawnTile) {
				SelectedTileClass = AvailableTileClasses[0];
				SafePlayerSpawnLocation = SpawnLocation + FVector(0.f, 0.f, 100.f);
			}
			else {
				int32 RandomIndex = Stream.RandRange(0, AvailableTileClasses.Num() - 1);
				SelectedTileClass = AvailableTileClasses[RandomIndex];
			}

			if (SelectedTileClass) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(SelectedTileClass, SpawnLocation, TileRotation, SpawnParams);

				if (SpawnedTile) {
					SpawnedTiles.Add(SpawnedTile);
				}
			}
		}
	}	
}

void AMapGenerator::ClearExistingMap()
{
	if (SpawnedTiles.Num() == 0) {
		return;
	}

	for (int32 i = SpawnedTiles.Num() - 1;i >= 0;i--) {
		AActor* TileToDestroy = SpawnedTiles[i];
	
		if (IsValid(TileToDestroy)) {
			TileToDestroy->Destroy();
		}
	}
	SpawnedTiles.Empty();
	
	UE_LOG(LogTemp, Log, TEXT("MapGenerator: Cleaned up existing map assets successfully."));
}

FVector AMapGenerator::GetPlayerSpawnLocation() const
{
	return SafePlayerSpawnLocation;
}