#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "Modifiers/CharacterModifiers/CharacterModifier.h"
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

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UCharacterModifier> ModClass : InitialModifiers) {
		AddCharacterModifier(ModClass);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (UCharacterModifier* Modifier : ActiveModifiers) {
		if (Modifier) {
			Modifier->OnTick(this, DeltaTime);
		}
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	float FilteredDamage = DamageAmount;
	
	for (UCharacterModifier* Modifier : ActiveModifiers) {
		if (Modifier) {
			FilteredDamage = Modifier->ModifyIncomingDamage(FilteredDamage);
		}
	}
	
	return Super::TakeDamage(FilteredDamage, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseCharacter::AddCharacterModifier(TSubclassOf<UCharacterModifier> ModifierClass)
{
	if (!ModifierClass) return;

	UCharacterModifier* NewMod = NewObject<UCharacterModifier>(this, ModifierClass);
	if (NewMod) {
		ActiveModifiers.Add(NewMod);
		NewMod->OnApply(this);
	}
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