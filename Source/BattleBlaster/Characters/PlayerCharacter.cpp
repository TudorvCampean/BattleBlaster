#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Modifiers/CharacterModifiers/CharacterModifier.h"
#include "Projectiles/Projectile.h"

APlayerCharacter::APlayerCharacter()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());
	BaseMesh->SetCanEverAffectNavigation(false);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	TurretMesh->SetCanEverAffectNavigation(false);

	TurretBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBarrelMesh"));
	TurretBarrelMesh->SetupAttachment(TurretMesh);
	TurretBarrelMesh->SetCanEverAffectNavigation(false);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretBarrelMesh);

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
}

void APlayerCharacter::Fire()
{
	if (!ProjectileClass || !ProjectileSpawnPoint) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), SpawnParams);
}

void APlayerCharacter::RotateTurretHorizontal(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void APlayerCharacter::AimBarrelVertical(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretBarrelMesh->GetComponentLocation();
	FVector LocalDirection = TurretMesh->GetComponentTransform().InverseTransformVector(VectorToTarget);
	float ClampedPitch = FMath::Clamp(LocalDirection.Rotation().Pitch, -10.0f, 90.0f);
	TurretBarrelMesh->SetRelativeRotation(FRotator(ClampedPitch, 0.0f, 0.0f));
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

void APlayerCharacter::MoveInput(const FInputActionValue& Value) { 
	AddMovementInput(GetActorForwardVector(), Value.Get<float>()); 
}

void APlayerCharacter::TurnInput(const FInputActionValue& Value) { 
	AddActorLocalRotation(FRotator(0.0f, Value.Get<float>() * 90.0f * GetWorld()->GetDeltaSeconds(), 0.0f), false); 
}

void APlayerCharacter::HandleJump() { 
	Jump();
}

void APlayerCharacter::HandleDestruction() { 
	Super::HandleDestruction(); 
	SetActorHiddenInGame(true); 
	SetActorTickEnabled(false);

	if (PlayerController) { 
		DisableInput(PlayerController); 
		PlayerController->SetShowMouseCursor(false); 
	} 
}