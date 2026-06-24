// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

APlayerPawn::APlayerPawn()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	IsAlive = true;
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	SetPlayerEnabled(true); // Activat direct pentru teste rapide
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController) {
		FHitResult HitResult;
		bool bHitSucces = PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult);

		FVector TargetLocation;

		if (bHitSucces) {
			TargetLocation = HitResult.ImpactPoint;
		}
		else {
			FVector MouseWorldLocation;
			FVector MouseWorldDirection;

			if (PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection)) {
				TargetLocation = MouseWorldLocation + (MouseWorldDirection * 10000.0f);
			}
			else {
				TargetLocation = GetActorLocation() + GetActorForwardVector() * 1000.0f;
			}
		}
		RotateTurretHorizontal(TargetLocation);
		AimBarrelVertical(TargetLocation);
	}
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &APlayerPawn::TurnInput);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerPawn::Fire);
	}
}

void APlayerPawn::Fire()
{
	if (!bCanFire) return;

	Super::Fire(); // Aici se spawnează glonțul și se injectează modificatorii din BasePawn!

	bCanFire = false;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerPawn::ResetFireCooldown, FireCooldown, false);
}

void APlayerPawn::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
	DeltaLocation.X = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalOffset(DeltaLocation, true);
}

void APlayerPawn::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);
	DeltaRotation.Yaw = TurnRate * InputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void APlayerPawn::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);
	IsAlive = false;
}

void APlayerPawn::SetPlayerEnabled(bool Enabled)
{
	if (PlayerController) {
		Enabled ? EnableInput(PlayerController) : DisableInput(PlayerController);
		PlayerController->SetShowMouseCursor(Enabled);
	}
}

void APlayerPawn::ResetFireCooldown()
{
	bCanFire = true;
}