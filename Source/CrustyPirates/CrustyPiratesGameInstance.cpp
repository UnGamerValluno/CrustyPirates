#include "CrustyPiratesGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UCrustyPiratesGameInstance::SetPlayerHP(int NewHP)
{
	PlayerHP = NewHP;
}

void UCrustyPiratesGameInstance::AddDiamond(int Amount)
{
	Diamonds += Amount;
}

void UCrustyPiratesGameInstance::KillEnemy()
{
	EnemiesPerLevel[CurrentLevelIndex - 1]--;
	UE_LOG(LogTemp, Warning, TEXT("Current level %d"), CurrentLevelIndex);
	UE_LOG(LogTemp, Warning, TEXT("Enemy killed %d"), EnemiesPerLevel[CurrentLevelIndex - 1]);
}

bool UCrustyPiratesGameInstance::IsLevelCleared()
{
	return EnemiesPerLevel[CurrentLevelIndex - 1] == 0;
}

void UCrustyPiratesGameInstance::RestartGame()
{
	Diamonds = 0;
	PlayerHP = 100;
	IsDoubleJumpUnlocked = false;

	ChangeLevel(1);
}

void UCrustyPiratesGameInstance::ChangeLevel(int LevelIndex)
{
	if (LevelIndex > 0)
	{
		CurrentLevelIndex = LevelIndex;
		FString LevelNameString = FString::Printf(TEXT("Level_%d"), LevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), FName(LevelNameString));
	}
}
