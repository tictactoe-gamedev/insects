// GPLv3


#include "IG_GameMode.h"

#include "IG_PlayerCharacter.h"
#include "IG_PlayerScoreWidget.h"
#include "IG_PlayerHud.h"
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

bool AIG_GameMode::IsPlayerAlive()
{
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		auto Player = Cast<AIG_PlayerCharacter>(PlayerController->GetPawn());
		if (Player)
		{
			return Player->CurrentHealth > 0;
		}
	}

	return false;
}
