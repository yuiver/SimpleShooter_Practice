// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "BhapticsSDK2.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = false;
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}

}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromViewport();
	//UE_LOG(LogTemp, Warning, TEXT("We Have Finished."));
	if (bIsWinner)
	{
		UBhapticsSDK2::PlayHaptic("win");
		UUserWidget* winScreen = CreateWidget(this, WinScreenClass);
		if (winScreen != nullptr)
		{
			winScreen->AddToViewport();
		}
	}
	else
	{
		UBhapticsSDK2::PlayHaptic("died");

		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}

	}
	bShowMouseCursor = true;
		//GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerController::RestartLevel, RestartDelay);
}