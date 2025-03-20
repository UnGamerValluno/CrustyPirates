#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"

#include "LevelExit.generated.h"

UCLASS()
class CRUSTYPIRATES_API ALevelExit : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* DoorFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* PlayerEnterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TargetLevelIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsActive = true;

	FTimerHandle WaitTimer;

	ALevelExit();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void OnWaitTimerTimeout();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
