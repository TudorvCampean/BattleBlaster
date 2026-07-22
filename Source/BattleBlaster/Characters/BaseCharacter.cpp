#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "Modifiers/CharacterModifiers/CharacterModifier.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
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


void ABaseCharacter::HandleDestruction()
{
	if (DestructionParticles) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestructionParticles, GetActorLocation(), GetActorRotation());
	if (DeathSound) UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}