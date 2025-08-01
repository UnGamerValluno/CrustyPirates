#include "CollectableItem.h"
#include "PlayerCharacter.h"

ACollectableItem::ACollectableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	ItemFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ItemFlipbook"));
	ItemFlipbook->SetupAttachment(RootComponent);
}

void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectableItem::OverlapBegin);
}

void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableItem::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player && Player->IsAlive)
	{
		Player->CollectItem(Type);
		Destroy();
	}
}
