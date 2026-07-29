#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleBlasterGameMode.generated.h"

class APlayerCharacter;
class ASpawnManager;

UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:	
	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<ASpawnManager> SpawnManagerClass;

private:	
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	ASpawnManager* SpawnManager;
};