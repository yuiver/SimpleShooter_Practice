// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	//A pawn Á×À½
	//UE_LOG(LogTemp, Warning, TEXT("You lose"));

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->GameHasEnded(nullptr, false);
	}

}
