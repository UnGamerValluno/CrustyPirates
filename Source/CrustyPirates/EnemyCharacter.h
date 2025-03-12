#pragma once

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class CRUSTYPIRATES_API AEnemyCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* PlayerDetectorSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextRenderComponent* HPText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayerCharacter* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistanceToTarget = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitPoints = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	AEnemyCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void UpdateHP(int NewHP);
	bool ShouldMoveToTarget();
	void UpdateDirection(float MoveDirection);
	void TakeDamage(int Damage, float StunDuration);

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
