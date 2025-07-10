#include "CrustyPiratesGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "LevelExit.h"
#include "PlayerCharacter.h"

ALevelExit::ALevelExit()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
	DoorFlipbook->SetupAttachment(RootComponent);
	DoorFlipbook->SetPlayRate(0.f);
	DoorFlipbook->SetLooping(false);
}

void ALevelExit::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UCrustyPiratesGameInstance>(GetGameInstance());
	check(GameInstance);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OverlapBegin);
}

void ALevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelExit::OnWaitTimerTimeout()
{
	GameInstance->ChangeLevel(TargetLevelIndex);
}

void ALevelExit::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player && Player->IsAlive && IsActive && GameInstance->IsLevelCleared())
	{
		IsActive = false;

		// Play door animation
		DoorFlipbook->SetPlayRate(1.f);
		DoorFlipbook->PlayFromStart();
		UGameplayStatics::PlaySound2D(GetWorld(), PlayerEnterSound);

		Player->Deactivate();
		GetWorldTimerManager().SetTimer(WaitTimer, this, &ALevelExit::OnWaitTimerTimeout, 1.f, false, WaitTime);
	}
}
