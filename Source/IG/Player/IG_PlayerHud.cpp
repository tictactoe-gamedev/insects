// GPLv3

#include "IG_PlayerHud.h"
#include "../IG_GameMode.h"
#include "Blueprint/WidgetTree.h"

void AIG_PlayerHud::BeginPlay()
{
	Super::BeginPlay();

	// Set up the healh bar
	HealthBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidget);
	HealthBarWidgetInstance->AddToViewport();

	// Set up the score text
	ScoreWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), ScoreWidget);
	ScoreWidgetInstance->AddToViewport();

	// Signal we're ready to the game mode
	AIG_GameMode* GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->HudReady();
}

void AIG_PlayerHud::ShowRestartButton()
{
	RestartButtonInstance = CreateWidget<UUserWidget>(GetWorld(), RestartButtonWidget);
	RestartButtonInstance->AddToViewport();
}
