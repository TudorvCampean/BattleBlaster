#include "ComputerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "AIController.h"

AComputerCharacter::AComputerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AttackStoppingDistance = 50.0f;
	AttackRange = 700.0f;
	AttackRate = 1.5f;
}

void AComputerCharacter::BeginPlay()
{
	Super::BeginPlay();
	AActor* BasePawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerTarget = Cast<APlayerCharacter>(BasePawn);

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AComputerCharacter::CheckAttackCondition, AttackRate, true);

	if (bShouldChasePlayer) ActivateEnemy();
}

void AComputerCharacter::ActivateEnemy()
{
	bShouldChasePlayer = true;
	GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &AComputerCharacter::UpdatePathfinding, 0.3f, true);
}

void AComputerCharacter::UpdatePathfinding()
{	
	if (PlayerTarget && bShouldChasePlayer)
	{
		AAIController* AICont = Cast<AAIController>(GetController());
		if (AICont) 
			AICont->MoveToActor(PlayerTarget, AttackStoppingDistance);
	}
}

bool AComputerCharacter::IsPlayerInAttackRange()
{
	if (!PlayerTarget) return false;
	return FVector::Dist(GetActorLocation(), PlayerTarget->GetActorLocation()) <= AttackRange;
}

void AComputerCharacter::CheckAttackCondition()
{
	if (PlayerTarget && IsPlayerInAttackRange())
	{
		Attack();
	}
}

void AComputerCharacter::HandleDestruction()
{
	GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	Super::HandleDestruction();
	Destroy();
}
