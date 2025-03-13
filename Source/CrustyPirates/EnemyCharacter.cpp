#include "EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	PlayerDetectorSphere->SetupAttachment(RootComponent);

	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HPText"));
	HPText->SetupAttachment(RootComponent);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHP(HitPoints);
	PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::DetectorOverlapBegin);
	PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::DetectorOverlapEnd);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && CanMove && FollowTarget && !IsStunned && ShouldMoveToTarget())
	{
		float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
		FVector WorldDirection = FVector(1.f, 0.f, 0.f);

		UpdateDirection(MoveDirection);
		AddMovementInput(WorldDirection, MoveDirection);
	}
}

void AEnemyCharacter::UpdateHP(int NewHP)
{
	HitPoints = NewHP;
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), HitPoints)));
}

void AEnemyCharacter::TakeDamage(int Damage, float StunDuration)
{
	if (IsAlive)
	{
		UpdateHP(HitPoints - Damage);
		auto AnimationNode = "JumpTakeHit";

		if (HitPoints <= 0.f)
		{
			UpdateHP(0);
			IsAlive = false;
			CanMove = false;
			AnimationNode = "JumpDie";
			HPText->SetHiddenInGame(true);
		}

		Stun(StunDuration);
		GetAnimInstance()->JumpToNode(FName(AnimationNode), FName("CrabbyStateMachine"));
	}
}

void AEnemyCharacter::OnStunTimerTimeout()
{
	IsStunned = false;
}

void AEnemyCharacter::Stun(float Duration)
{
	IsStunned = true;

	if (GetWorldTimerManager().IsTimerActive(StunTimer))
	{
		GetWorldTimerManager().ClearTimer(StunTimer);
	}

	GetWorldTimerManager().SetTimer(StunTimer, this, &AEnemyCharacter::OnStunTimerTimeout, 1.f, false, Duration);
	GetAnimInstance()->StopAllAnimationOverrides();
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
