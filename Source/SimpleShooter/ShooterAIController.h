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
	void StartBehaviorTree();
	bool IsDead() const;

protected:

	virtual void BeginPlay() override;

private:


	bool NeedToUpdatePath(float DistanceToPlayer);

	UPROPERTY(editAnywhere)
	class UBehaviorTree* AIBehavior;

	APawn* PlayerPawn;
	class AShooterCharacter* AIPawn;


#pragma region RecagyPrivate
	enum class EAIState
	{
		Idle,
		Chasing,
		LostTarget,
		Dead
	};


	bool IsPlayerInRange();
	EAIState CurrentState = EAIState::Idle;
	FVector LastKnownPlayerLocation;
	float MaxChaseDistance = 600;
	float AcceptanceRadius = 200;
	float LastPathUpdateTime = 0.0f;
	float PathUpdateInterval = 1.0f; // ��θ� ������ �ð� ���� (�� ����)
	float RequiredDistanceChange = 100.0f; // ��θ� �����ϱ� ���� �ʿ��� �ּ� �Ÿ� ��ȭ

#pragma endregion

};

