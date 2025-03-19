#include "CrustyPiratesGameInstance.h"

void UCrustyPiratesGameInstance::SetPlayerHP(int NewHP)
{
	PlayerHP = NewHP;
}

void UCrustyPiratesGameInstance::AddDiamond(int Amount)
{
	Diamonds += Amount;
}
