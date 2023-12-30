// GPLv3


#include "IG_RestartButtonWidget.h"
#include "IG_GameMode.h"

void UIG_RestartButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Register callback for button press
	Button->OnReleased.AddDynamic(this, &UIG_RestartButtonWidget::ButtonPressed);
}

void UIG_RestartButtonWidget::ButtonPressed()
{
	const auto GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->RestartGame();
}
