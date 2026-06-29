#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/ComputerCharacter.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* PawnActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerCharacter = Cast<APlayerCharacter>(PawnActor);
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	if (!DeadActor) return;
	
	if (DeadActor == PlayerCharacter)
	{
		PlayerCharacter->HandleDestruction();
	}
	
	else if (AComputerCharacter* DestroyedNPC = Cast<AComputerCharacter>(DeadActor))
	{
		DestroyedNPC->HandleDestruction();
	}
}