#include "ComputerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

AComputerCharacter::AComputerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AComputerCharacter::HandleDestruction()
{
	Super::HandleDestruction();
	
	Destroy();
}

void AComputerCharacter::BeginPlay()
{
	Super::BeginPlay();
	AActor* BasePawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerTarget = Cast<APlayerCharacter>(BasePawn);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AComputerCharacter::CheckAttackCondition, FireRate, true);
}

void AComputerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerTarget)
	{
		RotateTurretHorizontal(PlayerTarget->GetActorLocation());
		AimBarrelVertical(PlayerTarget->GetActorLocation());
	}
}

void AComputerCharacter::CheckAttackCondition()
{
	if (PlayerTarget && IsPlayerInAttackRange())
	{		
		Fire();
	}
}

bool AComputerCharacter::IsPlayerInAttackRange()
{
	if (!PlayerTarget) return false;
	float Distance = FVector::Dist(GetActorLocation(), PlayerTarget->GetActorLocation());
	return Distance <= FireRange;
}
