// GPLv3


#include "IG_GameMode.h"
#include "Player/IG_PlayerScoreWidget.h"
#include "Player/IG_PlayerHud.h"

#include "Kismet/GameplayStatics.h"

void AIG_GameMode::IncrementScore()
{
	PlayerScore++;

	// Grab the healbar from the gamemode
	AIG_PlayerHud* hud = Cast<AIG_PlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (hud)
	{
		auto ScoreWidget = Cast<UIG_PlayerScoreWidget>(hud->ScoreWidgetInstance);
		if (ScoreWidget)
		{
			const FString ScoreString = FString::Printf(TEXT("Score: %d"), PlayerScore);
			ScoreWidget->TextBlock->SetText(FText::AsCultureInvariant(ScoreString));
		}
	}
}

int AIG_GameMode::GetPlayerScore()
{
	return PlayerScore;
}

void AIG_GameMode::RestartGame()
{
	UE_LOG(LogGameMode, Warning, TEXT("Restarting game...."));
	UGameplayStatics::OpenLevel(GetWorld(), "Test1");
}

void AIG_GameMode::SetGameOver()
{
	GameOver = true;
	AIG_PlayerHud* hud = Cast<AIG_PlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (hud)
	{
		hud->ShowRestartButton();
	}
}

bool AIG_GameMode::GetGameOver()
{
	return GameOver;
}
