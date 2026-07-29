#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnManager.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/ComputerCharacter.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* PawnActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerCharacter = Cast<APlayerCharacter>(PawnActor);

	if (SpawnManagerClass)
	{
		SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	if (!DeadActor) return;
	
	if (DeadActor == PlayerCharacter)
	{
		if (SpawnManager) {
			SpawnManager->StopSpawning();
		}
		PlayerCharacter->HandleDestruction();

		return;
	}
	
	else if (AComputerCharacter* DestroyedNPC = Cast<AComputerCharacter>(DeadActor))
	{
		if (SpawnManager)
		{
			SpawnManager->OnEnemyKilled();
		}
		DestroyedNPC->HandleDestruction();
	}
}