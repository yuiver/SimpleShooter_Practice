// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "BhapticsSDK2.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger()
{
	//총구 이펙트
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleFlashSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (bSuccess)
	{
		//총알 맞추는 이펙트
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Hit.Location, ShotDirection.Rotation()); //방향은 빼도 된다고 함

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent PointDamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, PointDamageEvent, OwnerController, this);

			//플레이어 체크 로직
			APawn* HitPawn = Cast<APawn>(HitActor);
			if (HitPawn)
			{
				AController* HitController = HitPawn->GetController();
				APlayerController* PlayerController = Cast<APlayerController>(HitController);
				if (PlayerController)
				{	
					//플레이어가 맞다면 햅틱을 울린다.
					PlayerHapticOnHit(HitPawn ,Hit.Location);
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	//자신의 Trace를 무시하기 위한 코드
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

}

void AGun::PlayerHapticOnHit(APawn* HitPawn , FVector HitPoint)
{
	// 플레이어로부터 피격 위치까지의 방향 벡터를 계산합니다.
	FVector HitDirection = (HitPoint - HitPawn->GetActorLocation()).GetSafeNormal();


	// 플레이어의 전방 벡터를 얻습니다.
	FVector ForwardVector = HitPawn->GetActorForwardVector();


	// 두 벡터 사이의 각도를 계산합니다.
	float DotProduct = FVector::DotProduct(ForwardVector, HitDirection);
	float AngleRadians = acos(DotProduct); // acos는 라디안 값을 반환합니다.
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians); // 라디안을 도(degree)로 변환합니다.


	// 피격 위치가 플레이어의 왼쪽 또는 오른쪽인지 결정합니다.
	FVector CrossProduct = FVector::CrossProduct(ForwardVector, HitDirection);
	if (CrossProduct.Z < 0)
	{
		AngleDegrees = 360 - AngleDegrees; // 오른쪽의 각도를 양의 값으로 조정
	}
	// 반시계 방향으로 변환
	float CounterClockwiseAngle = 360.0f - AngleDegrees;

	// 그 결과로 햅틱을 울린다. "hit"을 변수화 시켜서 사용하는 로직도 괜찮을지도?
	UBhapticsSDK2::PlayHapticWithOption("hit", 1.0f, 1.0f, CounterClockwiseAngle, HitDirection.Z);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();
}

