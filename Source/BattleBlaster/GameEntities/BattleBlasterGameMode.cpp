// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Pawns/Tower.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) {
		Player = Cast<APlayerPawn>(PlayerPawn);
		if (Player == nullptr) {
			UE_LOG(LogTemp, Display, TEXT("Failed to find player tank!"));
		}
	}

	for (AActor* actor : Towers) {
		if (ATower* Tower = Cast<ATower>(actor)) {
			Tower->PlayerPawn = Player;
		}
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController) {
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget) {
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
		}
	}

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&ABattleBlasterGameMode::OnCountdownTimerTimeout,
		1,
		true
	);
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;
	if (CountdownSeconds > 0) {
		//UE_LOG(LogTemp, Display, TEXT("TIMER: %d"), CountdownSeconds);		
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0) {
		Player->SetPlayerEnabled(true);
		//UE_LOG(LogTemp, Display, TEXT("GO!"));
		ScreenMessageWidget->SetMessageText("Start!");
	}
	else {
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Player) {
		Player->HandleDestruction();
		IsGameOver = true;
	}
	else {
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower) {
			DeadTower->HandleDestruction();
			TowerCount--;
			if (TowerCount <= 0) {				
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}
	
	if (IsGameOver) {
		FString GameOverString;
		GameOverString = IsVictory ? "Victory!" : "Defeat!";
		//UE_LOG(LogTemp, Display, TEXT("%s"), *GameOverString);
		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(
			GameOverTimerHandle,
			this,
			&ABattleBlasterGameMode::OnGameOverTimerTimeout,
			GameOverDelay,
			false
		);
		
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);

		if (BattleBlasterGameInstance) {

			if (IsVictory) {
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else {
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}	
}
