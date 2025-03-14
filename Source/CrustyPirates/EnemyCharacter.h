#pragma once

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TimerHandle.h"
#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
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
	UBoxComponent* AttackCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayerCharacter* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AttackAnimSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistanceToTarget = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolDownInSeconds = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitPoints = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanAttack = true;

	FTimerHandle StunTimer;
	FTimerHandle AttackCoolDownTimer;
	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;

	AEnemyCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	bool ShouldMoveToTarget();
	void UpdateDirection(float MoveDirection);

	void UpdateHP(int NewHP);
	void Stun(float Duration);
	void OnStunTimerTimeout();
	void TakeDamage(int Damage, float StunDuration);

	void Attack();
	void OnAttackCoolDownTimerTimeout();
	void OnAttackOverrideAnimEnd(bool Completed);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox();

	UFUNCTION(BlueprintCallable)
	void DisableAttackCollisionBox();

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
