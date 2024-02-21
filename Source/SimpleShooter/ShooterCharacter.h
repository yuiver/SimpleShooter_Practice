// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void Shoot();


	UPROPERTY(EditAnywhere, Category = "Chara")
	float RotationRate = 70;

	UPROPERTY(EditDefaultsOnly, Category = "Chara")
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Chara")
	float Health;

	UPROPERTY(EditDefaultsOnly,Category = "Chara")
	TSubclassOf<AGun> GunClass;

	//UPROPERTY(EditDefaultsOnly)
	AGun* Gun;
};
