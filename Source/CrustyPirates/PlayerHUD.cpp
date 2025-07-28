#include "PlayerHUD.h"

void UPlayerHUD::SetHP(int HP)
{
	FString Str = FString::Printf(TEXT("HP: %d"), HP);
	HPText->SetText(FText::FromString(Str));
}

void UPlayerHUD::SetLevel(int Level)
{
	FString Str = FString::Printf(TEXT("Level: %d"), Level);
	LevelText->SetText(FText::FromString(Str));
}

void UPlayerHUD::SetDiamonds(int Diamonds)
{
	FString Str = FString::Printf(TEXT("Diamonds: %d"), Diamonds);
	DiamondsText->SetText(FText::FromString(Str));
}
