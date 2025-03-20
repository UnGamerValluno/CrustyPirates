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
