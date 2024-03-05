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
	//�ѱ� ����Ʈ
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleFlashSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (bSuccess)
	{
		//�Ѿ� ���ߴ� ����Ʈ
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Hit.Location, ShotDirection.Rotation()); //������ ���� �ȴٰ� ��

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent PointDamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, PointDamageEvent, OwnerController, this);

			//�÷��̾� üũ ����
			APawn* HitPawn = Cast<APawn>(HitActor);
			if (HitPawn)
			{
				AController* HitController = HitPawn->GetController();
				APlayerController* PlayerController = Cast<APlayerController>(HitController);
				if (PlayerController)
				{	
					//�÷��̾ �´ٸ� ��ƽ�� �︰��.
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

	//�ڽ��� Trace�� �����ϱ� ���� �ڵ�
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

}

void AGun::PlayerHapticOnHit(APawn* HitPawn , FVector HitPoint)
{
	// �÷��̾�κ��� �ǰ� ��ġ������ ���� ���͸� ����մϴ�.
	FVector HitDirection = (HitPoint - HitPawn->GetActorLocation()).GetSafeNormal();


	// �÷��̾��� ���� ���͸� ����ϴ�.
	FVector ForwardVector = HitPawn->GetActorForwardVector();


	// �� ���� ������ ������ ����մϴ�.
	float DotProduct = FVector::DotProduct(ForwardVector, HitDirection);
	float AngleRadians = acos(DotProduct); // acos�� ���� ���� ��ȯ�մϴ�.
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians); // ������ ��(degree)�� ��ȯ�մϴ�.


	// �ǰ� ��ġ�� �÷��̾��� ���� �Ǵ� ���������� �����մϴ�.
	FVector CrossProduct = FVector::CrossProduct(ForwardVector, HitDirection);
	if (CrossProduct.Z < 0)
	{
		AngleDegrees = 360 - AngleDegrees; // �������� ������ ���� ������ ����
	}
	// �ݽð� �������� ��ȯ
	float CounterClockwiseAngle = 360.0f - AngleDegrees;

	// �� ����� ��ƽ�� �︰��. "hit"�� ����ȭ ���Ѽ� ����ϴ� ������ ����������?
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

