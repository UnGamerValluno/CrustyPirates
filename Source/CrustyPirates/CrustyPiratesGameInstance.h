#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "CrustyPiratesGameInstance.generated.h"

UCLASS()
class CRUSTYPIRATES_API UCrustyPiratesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PlayerHP = 100;

	void SetPlayerHP(int NewHP);
};
