// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "BhapticsSDK2.h"

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	UBhapticsSDK2::PlayHaptic("died");

	UE_LOG(LogTemp, Warning, TEXT("We Have Finished."));

	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerController::RestartLevel, RestartDelay);
}