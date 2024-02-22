// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// Called every frame
void AShooterAIController::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	/* Behavior Service를 적용으로 주석으로 바꿈
	float DistanceToPlayer = FVector::Distance(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (PlayerPawn && LineOfSightTo(PlayerPawn) && DistanceToPlayer < MaxChaseDistance)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
	*/


	/* Behavior Tree 적용으로 주석으로 바꿈
	if (AIPawn && PlayerPawn)
	{
		if (!AIPawn->IsDead())
		{
			if (CurrentState != EAIState::Dead)
			{
				ClearFocus(EAIFocusPriority::Gameplay);
				StopMovement();
				CurrentState = EAIState::Dead;
			}
		}
		else 
		{
			float DistanceToPlayer = FVector::Distance(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
			if (DistanceToPlayer < MaxChaseDistance || (LineOfSightTo(PlayerPawn) && DistanceToPlayer < MaxChaseDistance * 3.0f))
			{
				if (CurrentState != EAIState::Chasing || NeedToUpdatePath(DistanceToPlayer))
				{
					SetFocus(PlayerPawn);
					MoveToActor(PlayerPawn, AcceptanceRadius);
					CurrentState = EAIState::Chasing;
				}
			}
			else if (CurrentState != EAIState::LostTarget)
			{
				ClearFocus(EAIFocusPriority::Gameplay);
				StopMovement();
				CurrentState = EAIState::LostTarget;
			}
		}
	}
	*/
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AIPawn = Cast<AShooterCharacter>(GetPawn());

	StartBehaviorTree();
}

void AShooterAIController::StartBehaviorTree()
{
	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}



//Tick에서 플레이어를 추적해야하는 거리인지 체크하는 로직
bool AShooterAIController::IsPlayerInRange()
{
	float DistanceToPlayer = FVector::Distance(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
	return DistanceToPlayer <= MaxChaseDistance; // MaxChaseDistance는 AI가 플레이어를 추적할 최대 거리
}



//Tick에서 플레이어를 추적해야하는지 시간과 이동거리로 체크하는 로직
bool AShooterAIController::NeedToUpdatePath(float DistanceToPlayer)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// 플레이어의 위치가 크게 변했는지, 또는 일정 시간이 지났는지 확인
	bool bTimeToUpdate = (CurrentTime - LastPathUpdateTime) > PathUpdateInterval;
	bool bSignificantLocationChange = FVector::Dist(LastKnownPlayerLocation, PlayerPawn->GetActorLocation()) > RequiredDistanceChange;

	if (bTimeToUpdate || bSignificantLocationChange)
	{
		// 갱신 조건을 만족하면, 마지막 갱신 시간과 플레이어의 위치를 업데이트
		LastPathUpdateTime = CurrentTime;
		LastKnownPlayerLocation = PlayerPawn->GetActorLocation();
		return true;
	}
	// 여기서 이동 경로를 갱신해야 할지 결정하는 로직 구현
	return false;
}
