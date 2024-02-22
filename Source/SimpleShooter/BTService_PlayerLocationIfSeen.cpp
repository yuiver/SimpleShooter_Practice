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

    // AI 컨트롤러 얻기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return;
    }

    // AI 컨트롤러가 제어하는 캐릭터 얻기
    AShooterCharacter* AIPawn = Cast<AShooterCharacter>(AIController->GetPawn());
    if (AIPawn == nullptr)
    {
        return;
    }

    // AI 캐릭터가 죽었는지 확인
    if (AIPawn->IsDead())
    {
        // AI 캐릭터가 죽었다면, 플레이어 위치 업데이트를 멈춤
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("StartLocation"));
        return;
    }

    // 플레이어 캐릭터 얻기
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr)
    {
        return;
    }

    // 플레이어 캐릭터에 대한 시야선(Line of Sight)이 있는지 확인
    if (AIController->LineOfSightTo(PlayerPawn))
    {
        // 시야선이 있다면, 플레이어의 위치를 블랙보드에 업데이트
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
    }
    else
    {
        // 시야선이 없다면, 플레이어 위치 정보를 블랙보드에서 제거
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }

}

