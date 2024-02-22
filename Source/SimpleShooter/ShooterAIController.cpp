// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// Called every frame
void AShooterAIController::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	/* Behavior Service�� �������� �ּ����� �ٲ�
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


	/* Behavior Tree �������� �ּ����� �ٲ�
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



//Tick���� �÷��̾ �����ؾ��ϴ� �Ÿ����� üũ�ϴ� ����
bool AShooterAIController::IsPlayerInRange()
{
	float DistanceToPlayer = FVector::Distance(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
	return DistanceToPlayer <= MaxChaseDistance; // MaxChaseDistance�� AI�� �÷��̾ ������ �ִ� �Ÿ�
}



//Tick���� �÷��̾ �����ؾ��ϴ��� �ð��� �̵��Ÿ��� üũ�ϴ� ����
bool AShooterAIController::NeedToUpdatePath(float DistanceToPlayer)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// �÷��̾��� ��ġ�� ũ�� ���ߴ���, �Ǵ� ���� �ð��� �������� Ȯ��
	bool bTimeToUpdate = (CurrentTime - LastPathUpdateTime) > PathUpdateInterval;
	bool bSignificantLocationChange = FVector::Dist(LastKnownPlayerLocation, PlayerPawn->GetActorLocation()) > RequiredDistanceChange;

	if (bTimeToUpdate || bSignificantLocationChange)
	{
		// ���� ������ �����ϸ�, ������ ���� �ð��� �÷��̾��� ��ġ�� ������Ʈ
		LastPathUpdateTime = CurrentTime;
		LastKnownPlayerLocation = PlayerPawn->GetActorLocation();
		return true;
	}
	// ���⼭ �̵� ��θ� �����ؾ� ���� �����ϴ� ���� ����
	return false;
}
