#pragma once

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperZDCharacter.h"

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

	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
