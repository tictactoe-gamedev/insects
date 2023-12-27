// GPLv3


#include "IG_RestartButtonWidget.h"
#include "IG_GameMode.h"

void UIG_RestartButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	Button->OnReleased.AddDynamic(this, &UIG_RestartButtonWidget::ButtonPressed);
}

void UIG_RestartButtonWidget::ButtonPressed()
{
	auto GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->RestartGame();
}
