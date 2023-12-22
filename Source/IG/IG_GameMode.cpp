// GPLv3


#include "IG_GameMode.h"
#include "Blueprint/WidgetTree.h"
#include "IG_PlayerHealthBar.h"

// Called when the game starts or when spawned
void AIG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidget);
	HealthBarWidgetInstance->AddToViewport();
}