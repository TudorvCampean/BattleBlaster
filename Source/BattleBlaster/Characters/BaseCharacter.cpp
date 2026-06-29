#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "Projectiles/Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	TurretBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBarrelMesh"));
	TurretBarrelMesh->SetupAttachment(TurretMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretBarrelMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseCharacter::RotateTurretHorizontal(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void ABaseCharacter::AimBarrelVertical(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretBarrelMesh->GetComponentLocation();
	FVector LocalDirection = TurretMesh->GetComponentTransform().InverseTransformVector(VectorToTarget);
	FRotator LocalLookAtRotation = LocalDirection.Rotation();

	float ClampedPitch = FMath::Clamp(LocalLookAtRotation.Pitch, -10.0f, 90.0f);
	TurretBarrelMesh->SetRelativeRotation(FRotator(ClampedPitch, 0.0f, 0.0f));
}

void ABaseCharacter::Fire()
{	
	if (!ProjectileClass || !ProjectileSpawnPoint) return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);
}

void ABaseCharacter::HandleDestruction()
{
	if (DestructionParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestructionParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}