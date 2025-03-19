#pragma once

#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"

#include "CollectableItem.generated.h"

UENUM(BlueprintType)
enum class CollectableType : uint8
{
	Diamond,
	HealthPotion,
	DoubleJumpUpgrade
};

UCLASS()
class CRUSTYPIRATES_API ACollectableItem : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* ItemFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	CollectableType Type;

	ACollectableItem();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
