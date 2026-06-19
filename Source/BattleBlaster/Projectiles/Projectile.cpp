// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));	

	TrailParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailParticles"));
	TrailParticles->SetupAttachment(RootComponent);

	InitialLifeSpan = 5.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner && OtherActor && OtherActor != this && OtherActor != GetOwner()) {
		
		if (OtherActor->IsA(APawn::StaticClass())) {
			UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, UDamageType::StaticClass());
		}	
		DisplayEffects(Hit);
	}
	Destroy();
}

void AProjectile::DisplayEffects(const FHitResult& Hit)
{	
	if (ImpactSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.ImpactPoint);
	}

	if (HitParticles) {
		FRotator ParticleRotation = Hit.ImpactNormal.Rotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitParticles,
			Hit.ImpactPoint,
			ParticleRotation
		);
	}
}
