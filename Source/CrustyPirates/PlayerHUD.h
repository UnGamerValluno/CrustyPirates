#pragma once

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerHUD.generated.h"

UCLASS()
class CRUSTYPIRATES_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HPText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DiamondsText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LevelText;

	void SetHP(int HP);
	void SetLevel(int Level);
	void SetDiamonds(int Diamonds);
};
