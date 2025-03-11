#include "EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	PlayerDetectorSphere->SetupAttachment(RootComponent);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::DetectorOverlapBegin);
	PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::DetectorOverlapEnd);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && CanMove && FollowTarget && ShouldMoveToTarget())
	{
		float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
		FVector WorldDirection = FVector(1.f, 0.f, 0.f);

		UpdateDirection(MoveDirection);
		AddMovementInput(WorldDirection, MoveDirection);
	}
}

bool AEnemyCharacter::ShouldMoveToTarget()
{
	bool ShouldMove = false;

	if (FollowTarget)
	{
		float DistanceToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
		ShouldMove = DistanceToTarget > StopDistanceToTarget;
	}

	return ShouldMove;
}

void AEnemyCharacter::UpdateDirection(float MoveDirection)
{
	FRotator CurrentRotation = GetActorRotation();
	float UpdatedRotation = (MoveDirection < 0.f) ? 180.f : 0.f;

	if (CurrentRotation.Yaw != UpdatedRotation)
	{
		SetActorRotation(FRotator(CurrentRotation.Pitch, UpdatedRotation, CurrentRotation.Roll));
	}
}

void AEnemyCharacter::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		FollowTarget = PlayerCharacter;
	}
}

void AEnemyCharacter::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		FollowTarget = NULL;
	}
}
