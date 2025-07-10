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
	int CurrentLevelIndex = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PlayerHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Diamonds = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDoubleJumpUnlocked = false;

	int EnemiesPerLevel[3] = { 1, 6, 8 };

	void SetPlayerHP(int NewHP);
	void AddDiamond(int Amount);

	void KillEnemy();

	UFUNCTION(BlueprintCallable)
	bool IsLevelCleared();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	void ChangeLevel(int LevelIndex);
};
