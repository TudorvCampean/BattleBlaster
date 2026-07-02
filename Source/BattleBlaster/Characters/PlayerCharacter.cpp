#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Modifiers/CharacterModifiers/CharacterModifier.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	JumpMaxCount = 1; 
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	for (TSubclassOf<UCharacterModifier> ModClass : InitialModifiers) {
		AddCharacterModifier(ModClass);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController) {
		FHitResult HitResult;
		if (PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult)) {
			RotateTurretHorizontal(HitResult.ImpactPoint);
			AimBarrelVertical(HitResult.ImpactPoint);
		}
	}

	for (UCharacterModifier* Modifier : ActiveModifiers) {
		if (Modifier) {
			Modifier->OnTick(this, DeltaTime);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnInput);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleJump);
	}
}

void APlayerCharacter::HandleDestruction()
{
	Super::HandleDestruction();
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	
	if (PlayerController)
	{
		DisableInput(PlayerController);
		PlayerController->SetShowMouseCursor(false);
	}
}

void APlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();	
	AddMovementInput(GetActorForwardVector(), InputValue);
}

void APlayerCharacter::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	AddActorLocalRotation(FRotator(0.0f, InputValue * 90.0f * GetWorld()->GetDeltaSeconds(), 0.0f), false);
}

void APlayerCharacter::HandleJump()
{	
	Jump();
}

void APlayerCharacter::AddCharacterModifier(TSubclassOf<UCharacterModifier> ModifierClass) {
	if (!ModifierClass)
		return;

	UCharacterModifier* NewMod = NewObject<UCharacterModifier>(this, ModifierClass);
	if (NewMod) {
		ActiveModifiers.Add(NewMod);
		NewMod->OnApply(this);
	}

}