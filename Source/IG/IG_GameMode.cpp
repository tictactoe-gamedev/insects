// GPLv3


#include "IG_GameMode.h"
#include "Player/IG_PlayerScoreWidget.h"
#include "Player/IG_PlayerHud.h"

#include "Kismet/GameplayStatics.h"

void AIG_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIG_GameMode::IncrementScore()
{
	PlayerScore++;

	// Try and get the score widget if we failed earlier due to race condition
	if (!ScoreWidget)
	{
		UE_LOG(LogGameMode, Error, TEXT("Still can't find the Score widget"));
		ScoreWidget = Cast<UIG_PlayerScoreWidget>(PlayerHud->ScoreWidgetInstance);
		return;
	}

	// Update the score widget
	const FString ScoreString = FString::Printf(TEXT("Score: %d"), PlayerScore);
	ScoreWidget->TextBlock->SetText(FText::AsCultureInvariant(ScoreString));
}

void AIG_GameMode::RestartGame()
{
	UE_LOG(LogGameMode, Warning, TEXT("Restarting game...."));
	UGameplayStatics::OpenLevel(GetWorld(), "Test1");
}

void AIG_GameMode::SetGameOver()
{
	// Register the gameover state
	GameOver = true;
	PlayerHud->ShowRestartButton();
}

void AIG_GameMode::HudReady()
{
	PlayerHud = Cast<AIG_PlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ScoreWidget = Cast<UIG_PlayerScoreWidget>(PlayerHud->ScoreWidgetInstance);
}
