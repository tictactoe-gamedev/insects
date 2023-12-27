// GPLv3

#include "IG_PlayerHud.h"
#include "Blueprint/WidgetTree.h"
#include "IG_PlayerHealthBar.h"

void AIG_PlayerHud::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidget);
	HealthBarWidgetInstance->AddToViewport();

	ScoreWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), ScoreWidget);
	ScoreWidgetInstance->AddToViewport();
}
