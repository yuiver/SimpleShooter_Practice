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
	FPS = CreateWidget(this, FPSClass);
	
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
	if (HUD != nullptr)
	{
		FPS->AddToViewport();
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

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleSettings", IE_Released, this, &AShooterPlayerController::ToggleSettings);
}

void AShooterPlayerController::ToggleSettings()
{
	if (SettingWidgetInstance == nullptr)
	{
		// 설정 창 위젯이 아직 표시되지 않았다면 생성하고 표시
		SettingWidgetInstance = CreateWidget<UUserWidget>(this, SettingClass);
		if (SettingWidgetInstance != nullptr)
		{
			SettingWidgetInstance->AddToViewport();
		}
	}
	else
	{
		// 이미 표시되었다면 제거
		SettingWidgetInstance->RemoveFromViewport();
		SettingWidgetInstance = nullptr;
	}
}
