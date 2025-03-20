#pragma once

#include "CollectableItem.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "CrustyPiratesGameInstance.h"
#include "Engine/TimerHandle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDCharacter.h"
#include "PlayerHUD.h"
#include "Sound/SoundBase.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class CRUSTYPIRATES_API APlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* AttackCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AttackAnimSequence;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerHUD* PlayerHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCrustyPiratesGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ItemCollectedSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int HitPoints = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsActive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int AttackDamage = 25;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AttackStunDuration = 0.5f;

	FTimerHandle StunTimer;
	FTimerHandle RestartGameTimer;
	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;

	APlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Deactivate();
	void UpdateHP(int NewHP);
	void Stun(float Duration);
	void OnStunTimerTimeout();
	void OnRestartGameTimerTimeout();
	void TakeDamage(int Damage, float StunDuration);

	void Move(const FInputActionValue& Value);
	void JumpStarted(const FInputActionValue& Value);
	void JumpEnded(const FInputActionValue& Value);	
	void UpdateDirection(float MoveDirection);

	void UnlockDoubleJump();
	void CollectItem(CollectableType ItemType);

	void Attack(const FInputActionValue& Value);
	void OnAttackOverrideAnimEnd(bool Completed);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox();

	UFUNCTION(BlueprintCallable)
	void DisableAttackCollisionBox();

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
