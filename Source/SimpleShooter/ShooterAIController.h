// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	enum class EAIState
	{
		Idle,
		Chasing,
		LostTarget,
		Dead
	};

	bool NeedToUpdatePath(float DistanceToPlayer);

	APawn* PlayerPawn;
	class AShooterCharacter* AIPawn;
	bool IsPlayerInRange();
	float MaxChaseDistance = 600;
	float AcceptanceRadius = 200;
	EAIState CurrentState = EAIState::Idle;

	float LastPathUpdateTime = 0.0f;
	float PathUpdateInterval = 1.0f; // ��θ� ������ �ð� ���� (�� ����)
	float RequiredDistanceChange = 100.0f; // ��θ� �����ϱ� ���� �ʿ��� �ּ� �Ÿ� ��ȭ
	FVector LastKnownPlayerLocation;

	UPROPERTY(editAnywhere)
	class UBehaviorTree* AIBehavior;
};

