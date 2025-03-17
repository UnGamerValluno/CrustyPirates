#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

APlayerCharacter:: APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// Attack animations
	DisableAttackCollisionBox();
	OnAttackOverrideEndDelegate.BindUObject(this, &APlayerCharacter::OnAttackOverrideAnimEnd);
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::AttackBoxOverlapBegin);

	// HUD
	if (PlayerHUDClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PlayerHUDClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToPlayerScreen();
			PlayerHUDWidget->SetHP(HitPoints);
			PlayerHUDWidget->SetLevel(1);
			PlayerHUDWidget->SetDiamonds(50);
		}
	}

	// Game instance
	GameInstance = Cast<UCrustyPiratesGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		HitPoints = GameInstance->PlayerHP;
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up input action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnded);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &APlayerCharacter::JumpEnded);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (IsAlive && CanMove && !IsStunned)
	{
		float MoveActionValue = Value.Get<float>();
		FVector Direction = FVector(1.f, 0.f, 0.f);
		AddMovementInput(Direction, MoveActionValue);
		UpdateDirection(MoveActionValue);
	}
}

void APlayerCharacter::UpdateDirection(float MoveDirection)
{
	FRotator CurrentRotation = Controller->GetControlRotation();
	float UpdatedRotation = (MoveDirection < 0.f) ? 180.f : 0.f;

	if (CurrentRotation.Yaw != UpdatedRotation)
	{
		Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, UpdatedRotation, CurrentRotation.Roll));
	}
}

void APlayerCharacter::JumpStarted(const FInputActionValue& Value)
{
	if (IsAlive && CanMove && !IsStunned)
	{
		Jump();
	}
}

void APlayerCharacter::JumpEnded(const FInputActionValue& Value)
{
	StopJumping();
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (IsAlive && CanAttack && !IsStunned)
	{
		CanAttack = false;
		CanMove = false;

		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("DefaultSlot"), 1.f, 0.f, OnAttackOverrideEndDelegate);
	}
}

void APlayerCharacter::UpdateHP(int NewHP)
{
	HitPoints = NewHP;
	PlayerHUDWidget->SetHP(HitPoints);
	GameInstance->SetPlayerHP(HitPoints);
}

void APlayerCharacter::Stun(float Duration)
{
	IsStunned = true;

	if (GetWorldTimerManager().IsTimerActive(StunTimer))
	{
		GetWorldTimerManager().ClearTimer(StunTimer);
	}

	DisableAttackCollisionBox();
	GetWorldTimerManager().SetTimer(StunTimer, this, &APlayerCharacter::OnStunTimerTimeout, 1.f, false, Duration);
	GetAnimInstance()->StopAllAnimationOverrides();
}

void APlayerCharacter::OnStunTimerTimeout()
{
	IsStunned = false;
}

void APlayerCharacter::TakeDamage(int Damage, float StunDuration)
{
	Stun(StunDuration);

	if (IsAlive)
	{
		UpdateHP(HitPoints - Damage);
		auto AnimationNode = "JumpTakeHit";

		if (HitPoints <= 0.f)
		{
			IsAlive = false;
			CanMove = false;
			CanAttack = false;
			AnimationNode = "JumpDie";

			UpdateHP(0);
			DisableAttackCollisionBox();
		}

		GetAnimInstance()->JumpToNode(FName(AnimationNode), FName("CaptainStateMachine"));
	}
}

void APlayerCharacter::OnAttackOverrideAnimEnd(bool Completed)
{
	CanAttack = true;
	CanMove = true;
}

void APlayerCharacter::EnableAttackCollisionBox()
{
	AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void APlayerCharacter::DisableAttackCollisionBox()
{
	AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void APlayerCharacter::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);

	if (Enemy)
	{
		Enemy->TakeDamage(AttackDamage, AttackStunDuration);
	}
}
