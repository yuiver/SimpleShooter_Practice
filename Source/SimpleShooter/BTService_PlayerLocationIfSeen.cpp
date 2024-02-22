// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // AI ��Ʈ�ѷ� ���
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return;
    }

    // AI ��Ʈ�ѷ��� �����ϴ� ĳ���� ���
    AShooterCharacter* AIPawn = Cast<AShooterCharacter>(AIController->GetPawn());
    if (AIPawn == nullptr)
    {
        return;
    }

    // AI ĳ���Ͱ� �׾����� Ȯ��
    if (AIPawn->IsDead())
    {
        // AI ĳ���Ͱ� �׾��ٸ�, �÷��̾� ��ġ ������Ʈ�� ����
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("StartLocation"));
        return;
    }

    // �÷��̾� ĳ���� ���
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr)
    {
        return;
    }

    // �÷��̾� ĳ���Ϳ� ���� �þ߼�(Line of Sight)�� �ִ��� Ȯ��
    if (AIController->LineOfSightTo(PlayerPawn))
    {
        // �þ߼��� �ִٸ�, �÷��̾��� ��ġ�� �����忡 ������Ʈ
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
    }
    else
    {
        // �þ߼��� ���ٸ�, �÷��̾� ��ġ ������ �����忡�� ����
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }

}

